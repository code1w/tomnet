#include "tomnet.h"
#include "asio_channel.h"
#include "asio_event_loop.h"
#include "event_loop_thread_pool.h"
#include "network_traffic.h"
#include "asio/asio.hpp"

#include <iostream>
#include <stdio.h>
#include <chrono>

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

			std::error_code ec;
			asio::ip::address_v4 addr(asio::ip::address_v4::from_string(ip, ec));
			socket_.async_connect(asio::ip::tcp::endpoint(addr, port),
				[this, tryconnect, self = shared_from_this()](const std::error_code& error)
			{

				if(error && connectedcb_)
				{
					connectedcb_(EVENT_CONNECT_FAIL);
				}
				else if(connectedcb_)
				{
					Start();
					connectedcb_(EVENT_CONNECT_SUCCESS); 
				}
			});
		}

		asio::ip::tcp::socket& AsioChannel::Socket()
		{
			return socket_;
		}

		void AsioChannel::AsyncReadError(const std::error_code& err)
		{
			switch(err.value())
			{
			case asio::error::connection_reset:
			case asio::error::eof:
			case asio::error::operation_aborted:
			case asio::error::bad_descriptor:
			case asio::error::connection_aborted:
			{
				CloseSocket();
				if(disconnectcb_)
				{
					disconnectcb_();
				}

				if(reconnectedcb_)
				{
					DoReConnect(remoteip_, remoteport_);
				}
			}
			break;
			default:
			break;
			}
		}

		void AsioChannel::SetSocketOpt()
		{
			asio::ip::tcp::no_delay no_delay(true);
			socket_.set_option(no_delay);
			asio::socket_base::receive_buffer_size recvbuf(MAX_PACKET_SIZE);
			socket_.set_option(recvbuf);
			asio::socket_base::receive_buffer_size sendbuf(MAX_PACKET_SIZE);
			socket_.set_option(sendbuf);
			asio::socket_base::reuse_address reuse(true);
			socket_.set_option(reuse);
			socket_.non_blocking(true);
		}

        void AsioChannel::ResetBuffer()
		{
			recvbuf_.retrieveAll();
		}

		void AsioChannel::Start()
		{
			if (!start_)
			{
				SetSocketOpt();
				tid_ = std::this_thread::get_id();
			    ResetBuffer();	
				AsyncRead();
				start_ = true;

#ifdef  TOM_NET_TRAFFIC
				NetworkTraffic::instance().FetchAddLinks();
#endif 
			}
		}

		void AsioChannel::AsyncRead()
		{
			socket_.async_read_some(asio::buffer(inputbuf_),
				[this, self = shared_from_this()](const std::error_code& err, size_t readsize)
			{
				if (err)
				{
					AsyncReadError(err);
					return;
				}

				recvbuf_.append(inputbuf_.data(), readsize);
				AsyncRead();
				TryCeneratePacket();
			});
		}

		void AsioChannel::TryCeneratePacket()
		{
			int readable = recvbuf_.readableBytes();

			while (readable > sizeof(int))
			{
				int packetlen = recvbuf_.peekInt32();
				if (readable < packetlen + sizeof(int))
				{
					break;
				}

				if (packetlen > 64 * 1024)
				{
					break;
				}

				packetlen = recvbuf_.readInt32();

				NetContext context;
				context.handler_ = handler_;
				context.evetype_ = EVENT_NETMSG;
				context.headerprotocal_ = nametype;
				context.ud_ = nullptr;

				std::shared_ptr<tom::Buffer> post = FethFreePackage();
				if(!post)
				{
					post = std::make_shared<tom::Buffer>();
				}
				post->ensureWritableBytes(sizeof(NetContext) + packetlen + sizeof(int));
				post->append(static_cast<const void*>(&context), sizeof(NetContext));
				post->appendInt32(packetlen);
				post->append(recvbuf_.peek(), packetlen);
				recvbuf_.retrieve(packetlen);
				readable = recvbuf_.readableBytes();
				PostPacket(post);
			}
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
#ifdef  TOM_NET_TRAFFIC
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
 
			auto fn = [this, packet]()
			{
				if (sendding_.load())
				{
					wbufferlist_.enqueue(packet);
				}
				else
				{
					AsyncSendData(packet);
				}
			};

			loop_->RunInIoService(std::move(fn));

			return 0;
		}


		void  AsioChannel::AsyncWriteSomeCallback(const std::error_code& error, const std::shared_ptr<tom::Buffer>& packet, std::size_t writen)
		{
			if (error)
			{
				if (errorcb_)
				{
					errorcb_(error.value());
				}

				return;
			}

#ifdef  TOM_NET_TRAFFIC
				NetworkTraffic::instance().FetchAddSendByte(writen);
#endif 
			if (writen < packet->readableBytes())
			{
				packet->retrieve(writen);
				AsyncSendData(packet);
			}
			else
			{
#ifdef  TOM_NET_TRAFFIC
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
#ifdef  TOM_NET_DEBUG
			assert(std::this_thread::get_id()== tid_);
			printf("Send size %d, tid %d \n",writen, tid_);
#endif

		}

		void AsioChannel::AsyncSendData(const std::shared_ptr<tom::Buffer>& packet)
		{
			sendding_.store(true);
			socket_.async_write_some(asio::buffer(packet->peek(), packet->readableBytes()),
				[this, packet](const std::error_code& err, std::size_t writen)
				{
					AsyncWriteSomeCallback(err,packet,writen);
				});
		}

		void AsioChannel::DoReConnect(const std::string& ip, uint16_t port)
		{ 
			recvbuf_.retrieveAll();
			start_ = false;
			std::error_code ec;
			asio::ip::address_v4 addr(asio::ip::address_v4::from_string(ip, ec));
			socket_.async_connect(asio::ip::tcp::endpoint(addr, port),
				[this, self = shared_from_this(), ip, port](const std::error_code& error){
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
			reconnectimer_.expires_from_now(std::chrono::milliseconds(RECONNECTTIME));
			reconnectimer_.async_wait(
				[this, self = shared_from_this(), ip, port](const std::error_code& error)
			{
				DoReConnect(ip ,port);
			});

		}

		void AsioChannel::CloseSocket()
		{
			socket_.cancel();
			std::error_code err;
			socket_.shutdown(asio::ip::tcp::socket::shutdown_both, err);
			socket_.close();
		}

		void AsioChannel::Close(uint32_t handle)
		{
			assert(handle == handler_);
			socket_.cancel();
			handler_ = 0;
			std::error_code err;
			socket_.shutdown(asio::ip::tcp::socket::shutdown_both, err);
			socket_.close();
		}

		void AsioChannel::FreePackage(const std::shared_ptr<tom::Buffer>& package)
		{
			package->retrieveAll();
			if (!freepacket_.enqueue(package))
			{
				// error
			}
			else {
				freeqsize_.fetch_add(1);
			}
		}

		std::shared_ptr<tom::Buffer> AsioChannel::FethFreePackage()
		{

			std::shared_ptr<tom::Buffer> empty;
			if (freeqsize_.load() > 0)
			{
				freepacket_.try_dequeue(empty);
				freeqsize_.fetch_sub(1);
			}
			return empty;
		}
}

}
