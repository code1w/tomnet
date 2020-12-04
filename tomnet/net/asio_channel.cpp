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
			std::cerr << "~AsioChannel" << '\n';
			std::shared_ptr<tom::Buffer> next;
			while (wbufferlist_.try_dequeue(next))
			{
				if(next)
				{
					next.reset();
				}
			}

			while (freepacket_.try_dequeue(next))
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
				[tryconnect, self = shared_from_this()](const std::error_code& error)
			{

				if(error && self->connectedcb_)
				{
					self->connectedcb_(EVENT_CONNECT_FAIL);
				}
				else if(self->connectedcb_)
				{
					self->Start();
					self->connectedcb_(EVENT_CONNECT_SUCCESS); 
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
				recycle_.store(true);
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
			try 
			{
				asio::ip::tcp::no_delay no_delay(true);
				socket_.set_option(no_delay);
				asio::socket_base::receive_buffer_size recvbuf(SOCKET_BUFSIZE);
				socket_.set_option(recvbuf);
				asio::socket_base::send_buffer_size sendbuf(SOCKET_BUFSIZE);
				socket_.set_option(sendbuf);
				asio::socket_base::reuse_address reuse(true);
				socket_.set_option(reuse);
				socket_.non_blocking(true);
				asio::socket_base::keep_alive keepalive(true);
				socket_.set_option(keepalive);
			}
			catch(const std::system_error& e)
			{
				std::cerr << e.what() << '\n';
			}
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
				[self = shared_from_this()](const std::error_code& err, size_t readsize)
			{
				try
				{
					if (err)
					{
						self->AsyncReadError(err);
						return;
					}
					self->recvbuf_.append(self->inputbuf_.data(), readsize);
					self->AsyncRead();
					self->TryCeneratePacket();
				}
				catch(const std::system_error& e)
				{
					std::cerr << e.what() << '\n';
				}
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
 

			auto fn = [self = shared_from_this(), packet]()
			{
				if (self->sendding_.load())
				{
					self->wbufferlist_.enqueue(packet);
				}
				else
				{
					self->AsyncSendData(packet);
				}
			};

			loop_->RunInIoService(std::move(fn));

			return 0;
		}

		int32_t AsioChannel::SendPacket(const std::shared_ptr<tom::Buffer>& packet, uint16_t size)
		{
			if (size > MAX_PACKET_SIZE)
			{
				if (errorcb_)
				{
					errorcb_( TOM_NET_ERROR_PACKET_INVALID);
				}
				return -1;
			}
 
			
			auto fn = [self = shared_from_this(), packet]()
			{
				if (self->sendding_.load())
				{
					self->wbufferlist_.enqueue(packet);
				}
				else
				{
					self->AsyncSendData(packet);
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
					//errorcb_(error.value());
				}

				return;
			}

			if (writen < packet->readableBytes())
			{
				packet->retrieve(writen);
				AsyncSendData(packet);
			}
			else
			{
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
		}

		void AsioChannel::AsyncSendData(const std::shared_ptr<tom::Buffer>& packet)
		{
			sendding_.store(true);
			socket_.async_write_some(asio::buffer(packet->peek(), packet->readableBytes()),
				[self = shared_from_this(), packet](const std::error_code& err, std::size_t writen)
				{
					self->AsyncWriteSomeCallback(err,packet,writen);
				});
		}

		void AsioChannel::DoReConnect(const std::string& ip, uint16_t port)
		{ 
			recvbuf_.retrieveAll();
			start_ = false;
			std::error_code ec;
			asio::ip::address_v4 addr(asio::ip::address_v4::from_string(ip, ec));

			socket_.async_connect(asio::ip::tcp::endpoint(addr, port),
				[self = shared_from_this(), ip, port](const std::error_code& error){
				if (!error)
				{
					self->reconnectimer_.cancel();
					self->Start();
					if(self->reconnectedcb_)
					{
						self->reconnectedcb_(EVENT_RECONNECTED); 
					}
				}
				else {

					self->DelayReConnect(ip , port);
				}

			});
		}

		void AsioChannel::DelayReConnect(const std::string& ip, uint16_t port)
		{
			reconnectimer_.expires_from_now(std::chrono::milliseconds(RECONNECTTIME));
			reconnectimer_.async_wait(
				[self = shared_from_this(), ip, port](const std::error_code& error)
			{
				self->DoReConnect(ip ,port);
			});

		}

		void AsioChannel::CloseSocket()
		{
			//socket_.cancel();
			auto fn = [this]()
			{
				handler_ = 0;
				std::error_code err;
				socket_.shutdown(asio::ip::tcp::socket::shutdown_both, err);
				socket_.close();
			};
			loop_->RunInIoService(std::move(fn));
		}

		void AsioChannel::Close(uint32_t handle)
		{
			//socket_.cancel();
			auto fn = [this]()
			{
				handler_ = 0;
				std::error_code err;
				socket_.shutdown(asio::ip::tcp::socket::shutdown_both, err);
				socket_.close();
			};
			loop_->RunInIoService(std::move(fn));
		}

		void AsioChannel::FreePackage(const std::shared_ptr<tom::Buffer>& package)
		{
			return ;
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
