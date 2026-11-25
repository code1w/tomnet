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
			loop_ = nullptr;
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

		// 每段连接生命周期内只通知一次断开,读/写错误路径都收敛到这里
		void AsioChannel::NotifyDisconnected()
		{
			if (disconnected_.exchange(true))
			{
				return;
			}
			auto handptr = handleweakptr_.lock();
			if (disconnectcb_ && handptr)
			{
				disconnectcb_();
			}
		}

		// 连接失效的公共处理: 通知上层断开, 客户端按配置重连
		void AsioChannel::HandleDisconnected()
		{
			NotifyDisconnected();
			auto handptr = handleweakptr_.lock();
			if (!handptr)
			{
				return;
			}
			if (reconnectedcb_)
			{
				// 客户端默认设置重连回调函数
				CloseSocket();
				auto self = shared_from_this();
				auto fn = [self](){
					self->DoReConnect(self->remoteip_, self->remoteport_);
				};
				loop_->RunInIoService(fn);
			}
			else
			{
				CloseSocket();
			}
		}

		void AsioChannel::HandleReadError(const std::error_code& err)
		{

			switch(err.value())
			{
			case asio::error::connection_reset:
			case asio::error::eof:
			{
				printf("TomNet AsioChannel AsyncReadError %d, %s", handler_, err.message().c_str());
				HandleDisconnected();
			}
			break;
			case asio::error::bad_descriptor:
			case asio::error::connection_aborted:
			case asio::error::operation_aborted:
			{
				// connection_aborted 是对端/协议栈中断(Windows 10053), 不是本端主动关闭,
				// 上层需要收到断开通知; 本端主动关闭时 closeing_ 已置位, 不重复通知
				if (err.value() == asio::error::connection_aborted && !closeing_.load())
				{
					NotifyDisconnected();
				}
				auto handptr = handleweakptr_.lock();
				if( errorcb_ && handptr)
				{
					errorcb_(err.value());
				}
				printf("TomNet AsioChannel AsyncReadError %d, %s", handler_, err.message().c_str());
			}
			break;
			default:
			{
				// 其它错误(如 ETIMEDOUT、WSAECONNRESET 变体等)按连接失效处理,
				// 否则连接会一直留在 handlers_ 里泄漏
				printf("TomNet AsioChannel AsyncReadError %d, %d, %s", handler_, err.value(), err.message().c_str());
				HandleDisconnected();
			}
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
				// 新一段连接生命周期, 允许再次通知断开
				disconnected_.store(false);
			}
			else 
			{
				printf("TomNet AsioChannel Repeat Start Error %d", handler_);
			}
		}

		void AsioChannel::AsyncRead()
		{
			socket_.async_read_some(asio::buffer(inputbuf_),
				std::bind(&AsioChannel::HandleRead, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
		}

		void AsioChannel::HandleRead(const std::error_code& error, size_t readsize)
		{
			if (error)
			{
				HandleReadError(error);
				return;
			}

			recvbuf_.append(inputbuf_.data(), readsize);
			AsyncRead();
			TryCeneratePacket();
		}

		void AsioChannel::TryCeneratePacket()
		{
			int32_t readable = recvbuf_.readableBytes();
			while (readable > sizeof(int32_t))
			{
				int32_t packetlen = recvbuf_.peekInt32();
				if (packetlen < 0 || packetlen > MAX_PACKET_SIZE)
				{
					printf("TomNet AsioChannel Bad PacketLen %d, %d", handler_, packetlen);
					CloseSocket();
					NotifyDisconnected();
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
				return -1;
			}
			auto packet = std::make_shared<tom::Buffer>();
			packet->ensureWritableBytes(size);
			packet->append(data, size);
			if(issending_)
			{
					sendbuffer_.push_back(packet);
			}
			else
			{
					AsyncSendData(packet);
			}
			return 0;
		}

		int32_t AsioChannel::SendPacket(std::shared_ptr<tom::Buffer> packet, uint16_t size)
		{
			if(closeing_.load())
			{
				return -1;
			}
			if(issending_)
			{
					sendbuffer_.push_back(packet);
			}
			else
			{
					AsyncSendData(packet);
			}
			return 0;
		}


		void  AsioChannel::HandleWrite(const std::error_code& error, std::size_t actuallen, std::shared_ptr<tom::Buffer> package)
		{
			if (error)
			{
				// 写失败后 socket 状态不可信,清空发送队列避免 issending_ 卡死、
				// 队列无限增长
				sendbuffer_.clear();
				issending_ = false;
				auto handptr = handleweakptr_.lock();
				if(socketwriteerrcb_ && handptr)
				{
					socketwriteerrcb_(error.value());
				}

				printf("GameshNet AsioChannel AsyncWriteSomeCallback  %d, Error %d", handler_, error.value() );
				// 服务端的写错误回调会关闭 socket, 挂起的读随之以 operation_aborted 结束,
				// 读路径就不会再通知断开, 所以这里要补发一次。
				// 带重连回调的客户端不在这里处理, 由读错误路径统一通知并重连。
				if (!reconnectedcb_ && !closeing_.load() && error.value() != asio::error::operation_aborted)
				{
					NotifyDisconnected();
				}
				return;
			}
			if(actuallen != package->readableBytes())
			{
				auto handptr = handleweakptr_.lock();
				if(socketwriteerrcb_ && handptr)
				{
					socketwriteerrcb_(error.value());
				}
				printf("GameshNet AsioChannel AsyncWriteSomeCallback  %d, Error %d", handler_, error.value() );
				return;
			}

			if(sendbuffer_.size() > 0)
			{
					auto it = sendbuffer_.front();
					sendbuffer_.pop_front();
					AsyncSendData(it);
			}
			else
			{
					issending_ = false;
			}

	

		}

		void AsioChannel::AsyncSendData(std::shared_ptr<tom::Buffer> package)
		{
			issending_=true;
			auto self(shared_from_this());
			asio::async_write(
						socket_, 
						asio::buffer(package->peek(), package->readableBytes()),
						[this, self, package](const std::error_code& ec, size_t actuallen){
						HandleWrite(ec, actuallen, package);
						});
		}

		void AsioChannel::DoReConnect(const std::string& ip, uint16_t port)
		{ 
			start_ = false;
			// 重连回调被清空表示用户已 CloseLink, 不再重连
			if (!reconnectedcb_)
			{
				return;
			}
			std::error_code ec;
			asio::ip::address_v4 addr(asio::ip::address_v4::from_string(ip, ec));
			auto self = shared_from_this();
			socket_.async_connect(asio::ip::tcp::endpoint(addr, port),
				[self, ip, port](const std::error_code& error){
				if (!self->reconnectedcb_)
				{
					// 重连过程中用户调用了 CloseLink: 这条连接已无人管理, 必须关掉
					std::error_code ignore;
					self->socket_.close(ignore);
					return;
				}
				if (!error)
				{
					self->reconnectimer_.cancel();
					self->closeing_.store(false);
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
