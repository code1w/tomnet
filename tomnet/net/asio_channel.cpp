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
		#ifdef DEBUG_NET
			printf("%s:%s\n",__FILE__, __FUNCTION__);
		#endif 
			while (!waitwriteq_.empty())
			{
				auto pkg = waitwriteq_.front(); 
				waitwriteq_.pop();
				pkg = nullptr;
			}

		}

		void AsioChannel::Connect(const std::string& ip, uint16_t port, bool tryconnect)
		{
			remoteip_  = ip;
			remoteport_ = port;

			std::error_code ec;
			asio::ip::address_v4 addr(asio::ip::address_v4::from_string(ip, ec));
			auto self = shared_from_this();
			socket_.async_connect(asio::ip::tcp::endpoint(addr, port),
				[self, tryconnect](const std::error_code& error)
			{

				if(error)
				{
					auto handptr = self->handleweakptr_.lock();
					if(self->connectedcb_ && handptr)
					{
						self->connectedcb_(EVENT_CONNECT_FAIL);
					}

					printf("TomNet AsioChannel Connect %d, Error %d ", self->handler_, error.value());
					return;
				}

				auto handptr = self->handleweakptr_.lock();
				if(self->connectedcb_ && handptr)
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
			{
				printf("TomNet AsioChannel AsyncReadError %d, %s", handler_, err.message().c_str());
				auto handptr = handleweakptr_.lock();
				if(disconnectcb_ && handptr)
				{
					disconnectcb_();
				}
				
				if(reconnectedcb_ && handptr)
				{
					// 客户端默认设置重连回调函数
					CloseSocket();
					auto self = shared_from_this();
					auto fn = [self](){
						self->DoReConnect(self->remoteip_, self->remoteport_);
					};
					loop_->RunInIoService(fn);
				}
			}
			break;
			case asio::error::bad_descriptor:
			case asio::error::connection_aborted:
			case asio::error::operation_aborted:
			{
				auto handptr = handleweakptr_.lock();
				if( errorcb_ && handptr)
				{
					errorcb_(err.value());
				}
				printf("TomNet AsioChannel AsyncReadError %d, %d", handler_, err.message().c_str());
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
				asio::socket_base::receive_buffer_size recvbuf(MAX_PACKET_SIZE);
				socket_.set_option(recvbuf);
				asio::socket_base::receive_buffer_size sendbuf(MAX_PACKET_SIZE);
				socket_.set_option(sendbuf);
				asio::socket_base::reuse_address reuse(true);
				socket_.set_option(reuse);
				socket_.non_blocking(true);
			}
			catch(const std::system_error& e)
			{
				printf("TomNet AsioChannel SetSocketOpt %d, Error %s", handler_, e.what() );
			}
			
		}


		void AsioChannel::Start()
		{
			if (socket_.is_open())
			{
				SetSocketOpt();
				tid_ = std::this_thread::get_id();
				AsyncRead();
				start_ = true;
			}
			else 
			{
				printf("TomNet AsioChannel Repeat Start Error %d", handler_);
			}
		}

		void AsioChannel::AsyncRead()
		{
			auto self = shared_from_this(); 
			socket_.async_read_some(asio::buffer(inputbuf_),
			[self](const std::error_code& err, size_t readsize)
			{
				if (err)
				{
					self->AsyncReadError(err);
					return;
				}

				self->recvbuf_.append(self->inputbuf_.data(), readsize);
				self->AsyncRead();
				self->TryCeneratePacket();
			});
		}

		void AsioChannel::TryCeneratePacket()
		{
			int32_t readable = recvbuf_.readableBytes();
			while (readable > sizeof(int32_t))
			{
				int32_t packetlen = recvbuf_.peekInt32();
				if (packetlen < 0 || packetlen > MAX_PACKET_SIZE)
				{
					auto handptr = handleweakptr_.lock();
					if(disconnectcb_ && handptr)
					{
						disconnectcb_();
					}
					return;
				}

				if (readable < packetlen + sizeof(int32_t))
				{
					break;
				}

				packetlen = recvbuf_.readInt32();
				NetContext context;
				context.handler_ = handler_;
				context.evetype_ = EVENT_NETMSG;
				context.headerprotocal_ = nametype;
				context.ud_ = nullptr;

				auto package = std::make_shared<tom::Buffer>();
				package->ensureWritableBytes(sizeof(NetContext) + packetlen + sizeof(int32_t));
				package->append(static_cast<const void*>(&context), sizeof(NetContext));
				package->appendInt32(packetlen);
				package->append(recvbuf_.peek(), packetlen);
				recvbuf_.retrieve(packetlen);

				readable = recvbuf_.readableBytes();
				PostPacket(package);
			}
		}

		uint32_t AsioChannel::PostPacket(const std::shared_ptr<tom::Buffer>& packet)
		{
			NetContext* context = (NetContext*)packet->peek();
			switch (context->evetype_)
			{
			case EVENT_NETMSG:
			{
				auto handptr = handleweakptr_.lock();
				if (messagecb_ && handptr)
				{
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


		int32_t AsioChannel::SendPacket(const char* data, uint32_t size)
		{
			if(closeing_.load())
			{
				// 正在关闭禁止发包
				return -1;
			}
			
			auto packet = std::make_shared<tom::Buffer>();
			packet->ensureWritableBytes(size);
			packet->append(data, size);
 
			if (sendding_.load())
			{
				waitwriteq_.push(packet);
			}
			else
			{
				AsyncSendData(packet);
			}
			return 0;
		}

		int32_t AsioChannel::SendPacket(const std::shared_ptr<tom::Buffer>& packet, uint16_t size)
		{

			if(closeing_.load())
			{
				// 正在关闭禁止发包
				return -1;
			}


			if (sendding_.load())
			{
				waitwriteq_.push(packet);
			}
			else
			{
				AsyncSendData(packet);
			}

			return 0;
		}


		void  AsioChannel::AsyncWriteSomeCallback(const std::error_code& error, const std::shared_ptr<tom::Buffer>& packet, std::size_t writen)
		{
			if (error)
			{
				auto handptr = handleweakptr_.lock();
				if(socketwriteerrcb_ && handptr)
				{
					socketwriteerrcb_(error.value());
				}

				printf("TomNet AsioChannel AsyncWriteSomeCallback  %d, Error %d", handler_, error.value() );
				return;
			}
 
			if (writen < packet->readableBytes())
			{
				packet->retrieve(writen);
				AsyncSendData(packet);
			}
			else
			{
				if(!waitwriteq_.empty())
				{
					auto next = waitwriteq_.front();
					waitwriteq_.pop();
					AsyncSendData(next);
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
			auto self = shared_from_this();
			socket_.async_write_some(asio::buffer(packet->peek(), packet->readableBytes()),
				[self, packet](const std::error_code& err, std::size_t writen)
				{
					self->AsyncWriteSomeCallback(err,packet,writen);
				});
		}

		void AsioChannel::DoReConnect(const std::string& ip, uint16_t port)
		{ 
			start_ = false;
			std::error_code ec;
			asio::ip::address_v4 addr(asio::ip::address_v4::from_string(ip, ec));
			auto self = shared_from_this();
			socket_.async_connect(asio::ip::tcp::endpoint(addr, port),
				[self, ip, port](const std::error_code& error){
				if (!error)
				{
					self->reconnectimer_.cancel();
					//self->closeing_.store(false);
					self->Start();
					auto handptr = self->handleweakptr_.lock();
					if(self->reconnectedcb_ && handptr)
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
			auto self = shared_from_this();
			reconnectimer_.async_wait(
				[self, ip, port](const std::error_code& error)
			{
				self->DoReConnect(ip ,port);
			});

		}

		
		void AsioChannel::CloseSocket()
		{
			try
			{
				#ifdef DEBUG_NET
				printf("%s:%s:%d\n",__FILE__, __FUNCTION__,__LINE__);
				#endif 
				if(socket_.is_open() && !closeing_.load())
				{
					#ifdef DEBUG_NET
					printf("%s:%s:%d\n",__FILE__, __FUNCTION__,__LINE__);
					#endif 
					printf("TomNet AsioChannel CloseSocket %d", handler_);
					closeing_.store(true);
					auto self = shared_from_this();
					socket_.cancel();
					std::error_code shutdownerr;
					self->socket_.shutdown(asio::ip::tcp::socket::shutdown_both, shutdownerr);
					if(shutdownerr)
					{
						#ifdef DEBUG_NET
						// shutdownerr.value() = 107
						printf("%s:%s:%d, %d, %d \n",__FILE__, __FUNCTION__,__LINE__,shutdownerr.value(), self->handler_);
						#endif 
						printf("TomNet AsioChannel CloseSocket %d, Socket ShutDown Error %d", handler_, shutdownerr.value());
					}
					std::error_code closeerr;
					self->socket_.close(closeerr);
					if(closeerr)
					{
						#ifdef DEBUG_NET
						printf("%s:%s:%d, %d ,%d \n",__FILE__, __FUNCTION__,__LINE__,closeerr.value(), self->handler_);
						#endif 
						printf("TomNet AsioChannel CloseSocket %d, Socket Close Error %d", handler_, closeerr.value());
					}
				}
			}
			catch(const std::system_error& e)
			{
				printf("TomNet AsioChannel CloseSocket Catch Error %s", e.what());
			}
			
		}

		// 关闭服务器时清理回调
		void AsioChannel::CleanCallback()
		{
			connectedcb_ = nullptr;
			reconnectedcb_ = nullptr;
			messagecb_ = nullptr;
			errorcb_ = nullptr;
			disconnectcb_ = nullptr;
		}

}

}
