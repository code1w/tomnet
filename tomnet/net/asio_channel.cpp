#include "tomnet.h"
#include "asio_channel.h"
#include "asio_event_loop.h"
#include "event_loop_thread_pool.h"
#include "network_traffic.h"
//#include "base/codec.h"

#include "boost/asio.hpp"
//#include "asio/include/asio.hpp"
#include "boost/system/system_error.hpp"
#include "boost/bind/bind.hpp"

#include <iostream>
#include <stdio.h>

namespace tom
{
	namespace net
	{

		AsioChannel::AsioChannel(AsioEventLoop* loop, std::size_t block_size)
			:socket_(loop->io_service())
			, block_size_(block_size)
			,loop_(loop)
			,reconnectimer_(loop->io_service())

		{
		}

		AsioChannel::~AsioChannel()
		{
			std::shared_ptr<tom::Buffer> next;
			while (wbufferlist_.try_dequeue(next))
			{
				if(next)
				{
					next.reset();
				}
			}

		}

		void AsioChannel::Connect(const std::string& ip, uint16_t port, bool tryconnect)
		{
			remoteip_  = ip;
			remoteport_ = port;

			boost::system::error_code ec;
			boost::asio::ip::address_v4 addr(boost::asio::ip::address_v4::from_string(ip, ec));
			socket_.async_connect(boost::asio::ip::tcp::endpoint(addr, port),
				[this, tryconnect, self = shared_from_this()](const boost::system::error_code& error)
			{

				if(error)
				{
					if(connectedcb_)
					{
						connectedcb_(EVENT_CONNECT_FAIL);
					}
					return;
				}

				if(connectedcb_)
				{
					Start();
					connectedcb_(EVENT_CONNECT_SUCCESS); 
				}
			});
		}

		boost::asio::ip::tcp::socket& AsioChannel::Socket()
		{
			return socket_;
		}

		void AsioChannel::AsyncReadError(const boost::system::error_code& err)
		{
			switch(err.value())
			{
			case boost::asio::error::connection_reset:
			{
				CloseSocket();
				if(disconnectcb_)
				{
					disconnectcb_();
				}
				
				DoReConnect(remoteip_, remoteport_);
			}
			break;
			case boost::asio::error::eof:
			case boost::asio::error::operation_aborted:
			case boost::asio::error::bad_descriptor:
			case boost::asio::error::connection_aborted:
			default:
			break;
			}
		}

		void AsioChannel::SetSocketOpt()
		{
			boost::asio::ip::tcp::no_delay no_delay(true);
			socket_.set_option(no_delay);
			boost::asio::socket_base::receive_buffer_size recvbuf(MAX_PACKET_SIZE);
			socket_.set_option(recvbuf);
			boost::asio::socket_base::receive_buffer_size sendbuf(MAX_PACKET_SIZE);
			socket_.set_option(sendbuf);
			boost::asio::socket_base::reuse_address reuse(true);
			socket_.set_option(reuse);
		}


		void AsioChannel::Start()
		{
			if (!start_)
			{
				SetSocketOpt();
				tid_ = std::this_thread::get_id();
     			ReadPacketLen();
				start_ = true;

#ifdef  TOM_ TOM_NET_TRAFFIC
				NetworkTraffic::instance().FetchAddLinks();
#endif 
			}
		}

		void AsioChannel::ReadPacketLen()
		{
			nsize_ = 0;
			boost::asio::async_read(socket_, boost::asio::buffer(&nsize_, sizeof(int32_t)),
				[this,self = shared_from_this()](
					const boost::system::error_code& err, size_t cb){

				if(err)
				{
					AsyncReadError(err);
					return;
				}

				int32_t hsize = ntohl(nsize_);
				if (hsize + sizeof(int32_t) > MAX_PACKET_SIZE)
				{
					socket_.cancel();
					if(disconnectcb_)
					{
						disconnectcb_();
					}
					Close(handler_);
					return;
				}
				
				ReadPacketBody(hsize);
			});
		}

		void AsioChannel::ReadPacketBody(size_t bsize)
		{
			auto post = std::make_shared<tom::Buffer>();

#ifdef  TOM_ TOM_NET_TEST
			post->ensureWritableBytes(bsize + 10);
			post->appendInt32(bsize);
#else 
			NetContext context;
			context.handler_ = handler_;
			context.evetype_ = EVENT_NETMSG;
			context.headerprotocal_ = nametype;
			context.ud_ = nullptr;
			post->ensureWritableBytes(sizeof(NetContext) + bsize + sizeof(bsize));
			post->append(static_cast<const void*>(&context), sizeof(NetContext));
			post->appendInt32(bsize);
#endif 
			boost::asio::async_read(socket_, boost::asio::buffer(post->beginWrite(), bsize),
				[this,self = shared_from_this(), bsize, post](
					const boost::system::error_code& err, size_t cb){
				if (!err)
				{
					assert(cb == bsize);
					post->hasWritten(bsize);
#ifdef  TOM_ TOM_NET_TRAFFIC
					NetworkTraffic::instance().FetchAddRecvPacket();
					NetworkTraffic::instance().FetchAddRecvByte(bsize);
#endif 

#ifdef  TOM_ TOM_NET_DEBUG
					int32_t recv_size = bsize + sizeof(MessageHeader);
					assert(std::this_thread::get_id() == tid_);
					printf("Recv Size %d, tid %d , handle %d \n",recv_size, tid_,handler_);
#endif 

					ReadPacketLen();

#ifdef  TOM_ TOM_NET_TEST
					SendPacket(post->peek(), post->readableBytes());
#else
					PostPacket(post);
#endif 
				}
				else 
				{
					AsyncReadError(err);
				}
			});

		}

		uint32_t AsioChannel::PostPacket(const std::shared_ptr<tom::Buffer>& packet)
		{
			NetContext* context = (NetContext*)packet->peek();
			switch (context->evetype_)
			{
			case EVENT_NETMSG:
			{
				if (messagecb_)
				{
#ifdef  TOM_ TOM_NET_TRAFFIC
					NetworkTraffic::instance().FetchAddPostMsg();
#endif 
					messagecb_(packet);
				}
				failed_count_ = failed_count_ > 0 ? (failed_count_ - 1) : 0;
			}
			break;
			default:
				break;

			}
			return context->evetype_;
		}


		int32_t AsioChannel::SendPacket(const char* data, uint16_t size)
		{
			if (size > MAX_PACKET_SIZE)
			{
				if (errorcb_)
				{
					errorcb_( TOM_NET_ERROR_PACKET_INVALID);
				}
				return -1;
			}
			
			auto packet = std::make_shared<tom::Buffer>();
			packet->ensureWritableBytes(size);
			packet->append(data, size);
 
			if (sendding_.load())
			{
				wbufferlist_.enqueue(packet);
			}
			else
			{
				AsyncSendData(packet);
			}

			return 0;
		}


		void  AsioChannel::AsyncWriteSomeCallback(const boost::system::error_code& error, const std::shared_ptr<tom::Buffer>& packet, std::size_t writen)
		{
			if (error)
			{
				if (errorcb_)
				{
					errorcb_(error.value());
				}

				return;
			}

#ifdef  TOM_ TOM_NET_TRAFFIC
				NetworkTraffic::instance().FetchAddSendByte(writen);
#endif 
			if (writen < packet->readableBytes())
			{
				packet->retrieve(writen);
				AsyncSendData(packet);
			}
			else
			{
#ifdef  TOM_ TOM_NET_TRAFFIC
				NetworkTraffic::instance().FetchAddSendPacket();
#endif 
				if (wbufferlist_.size_approx() != 0)
				{
					std::shared_ptr<tom::Buffer> next;
					wbufferlist_.try_dequeue(next);
					if(next)
					{
						AsyncSendData(next);
					}
				}
				else
				{
					sendding_.store(false);
				}
			}
#ifdef  TOM_ TOM_NET_DEBUG
			assert(std::this_thread::get_id()== tid_);
			printf("Send size %d, tid %d \n",writen, tid_);
#endif

		}

		void AsioChannel::AsyncSendData(const std::shared_ptr<tom::Buffer>& packet)
		{

			sendding_.store(true);
			socket_.async_write_some(boost::asio::buffer(packet->peek(), packet->readableBytes()),
				boost::bind(&AsioChannel::AsyncWriteSomeCallback, this,
					boost::asio::placeholders::error, packet,
					boost::asio::placeholders::bytes_transferred));
		}

		void AsioChannel::DoReConnect(const std::string& ip, uint16_t port)
		{ 
			start_ = false;
			boost::system::error_code ec;
			boost::asio::ip::address_v4 addr(boost::asio::ip::address_v4::from_string(ip, ec));
			socket_.async_connect(boost::asio::ip::tcp::endpoint(addr, port),
				[this, self = shared_from_this(), ip, port](const boost::system::error_code& error){
				if (!error)
				{
					reconnectimer_.cancel();
					Start();
					if(reconnectedcb_)
					{
						reconnectedcb_(EVENT_RECONNECTED); 
					}
				}
				else {

					DelayReConnect(ip , port);
				}

			});
		}

		void AsioChannel::DelayReConnect(const std::string& ip, uint16_t port)
		{
			reconnectimer_.expires_from_now(boost::posix_time::millisec(RECONNECTTIME));
			reconnectimer_.async_wait(
				[this, self = shared_from_this(), ip, port](const boost::system::error_code& error)
			{
				DoReConnect(ip ,port);
			});

		}

		void AsioChannel::CloseSocket()
		{
			socket_.cancel();
			boost::system::error_code err;
			socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, err);
			socket_.close();

		}

		void AsioChannel::Close(uint32_t handle)
		{
			assert(handle == handler_);
			socket_.cancel();
			handler_ = 0;
			boost::system::error_code err;
			socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, err);
			socket_.close();
		}

	
}

}
