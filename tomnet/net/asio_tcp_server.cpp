#include "asio_tcp_server.h"
#include "asio_server_handler.h"
#include "handler_manager.h"
#include "asio_server_handler.h"
#include "event_loop_thread_pool.h"
#include "asio_event_loop.h"
#include "net_define.h"

#include <stdio.h>

namespace tom
{
	namespace net
	{
		AsioTcpServer::AsioTcpServer(AsioEventLoop* loop, IMessageQueue* msgqueue,  uint32_t maxconn)
			:max_connect_(maxconn)
			,msgqueue_(msgqueue)
			,loop_(loop)
		{
			
		}
		
		AsioTcpServer::AsioTcpServer(AsioEventLoop* loop,uint32_t maxconn)
			:max_connect_(maxconn)
			,loop_(loop)
		{
			
		}

		bool AsioTcpServer::Start(const char* ip, uint16_t port)
		{
			std::error_code ec;
			auto endpoint = asio::ip::tcp::endpoint(asio::ip::address::from_string(ip), port);
			acceptorptr_.reset(new asio::ip::tcp::acceptor(accept_thread_loop_.loop()->io_service()));
			acceptorptr_->open(endpoint.protocol());
			acceptorptr_->set_option(asio::ip::tcp::acceptor::reuse_address(reuse_addr_));
			acceptorptr_->bind(endpoint, ec);
			if (ec) 
			{
				printf("TomNet AsioTcpServer Start Error %s:%d, %s", ip,port, ec.message().c_str());
				return false;
			}

			auto accept_io_thread_run = [this,ip,port]() -> int {
				printf("TomNet AsioTcpServer Accept IO Thread Runing %s:%d", ip,port);
				return 0;
			};

			auto accept_io_thread_stop = [this,ip,port]()->int {
				printf("TomNet AsioTcpServer Accept IO Thread Stoped %s:%d", ip,port);
				return 0;
			};
			
			accept_thread_loop_.Start(true, accept_io_thread_run, accept_io_thread_stop);
			try
			{
				acceptorptr_->listen();
				
			}
			catch(const std::error_code& ec)
			{
				printf("TomNet AsioTcpServer Listen Error %s:%d, %s", ip, port, ec.message().c_str());
				return false;
			}
			
			ip_ = ip;
			port_ = port;
			srvkey_ = ip_ + ":" + std::to_string(port_);

			// accept 移动到IO线程执行
			auto fn = [this](){
				Accept();
			};
			accept_thread_loop_.loop()->RunInIoService(std::move(fn));
			return true;
		}


		void AsioTcpServer::Accept()
		{
			uint64_t handle = HandlerManager::instance().AllocateHandlerId();
			auto loop = EventLoopThreadPool::instance().GetNextLoopWithHash(handle);
			if(!loop)
			{
				printf("TomNet AsioTcpServer Accept GetNextLoopWithHash Error %d", handle);
				return;
			}
			
			auto handler = std::make_shared<AsioServerHandler>(loop, msgqueue_);
			if(!handler)
			{
				printf("TomNet AsioTcpServer Accept New ServerHandler Error %d", handle);
				return;
			}
			handler->SetHandler(handle);
			handler->SetMsgHeaderProtocal(headprotol_);
			handler->SetUserData(GetUserdata());
			if(messagecb_)
			{
				handler->SetMessageCb(messagecb_);
			}
			loop->AddHandler(handle, handler);
			acceptorptr_->async_accept(handler->Socket(),
				[this,handler](const std::error_code& error)
				{
					if (!error)
					{
						AsyncAcceptCallback(handler);
					}
					else
					{
						printf("TomNet AsioTcpServer Asio async_accept  Error %s", error.message().c_str());
					}
					
				});
		}

		void AsioTcpServer::AsyncAcceptCallback(const std::shared_ptr<AsioServerHandler>& handler)
		{
			auto handle = handler->GetHandler();
			auto loop = EventLoopThreadPool::instance().GetNextLoopWithHash(handle);
			if(!loop)
			{
				printf("TomNet AsioTcpServer AsyncAcceptCallback GetNextLoopWithHash Error !");
				return;
			}

			auto fn = [handler](){
				handler->OnAccept();
			};
			loop->RunInIoService(std::move(fn));
			
			printf("TomNet AsioTcpServer AsyncAcceptCallback  handle : %d, loop:  %p", handle, loop);
			myhandlers_.push_back(handle); // 连接到这个端口的连接
			max_connect_++;
			Accept();
		}


		AsioEventLoop* AsioTcpServer::GetAcceptLoop()
		{
			return accept_thread_loop_.loop();
		}

		void AsioTcpServer::Stop()
		{
			accept_thread_loop_.Stop(true);
		}

	}
}