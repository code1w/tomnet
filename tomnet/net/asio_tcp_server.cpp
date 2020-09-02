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

		bool AsioTcpServer::Start(const char* address, uint16_t port)
		{
			accept_thread_loop_.Start();
			std::error_code ec;
			auto endpoint = asio::ip::tcp::endpoint(asio::ip::address::from_string(address), port);
			acceptorptr_.reset(new asio::ip::tcp::acceptor(accept_thread_loop_.loop()->io_service()));
			acceptorptr_->open(endpoint.protocol());
			acceptorptr_->set_option(asio::ip::tcp::acceptor::reuse_address(reuse_addr_));
			acceptorptr_->bind(endpoint, ec);
			if (ec) 
			{
				return false;
			}
			acceptorptr_->listen();
			auto fn = [this]()
			{
				Accept();
			};
			
			for(int i = 0; i < MAXACCEPTPOOl; ++i)
			{
				accept_thread_loop_.loop()->RunInIoService(std::move(fn));
			}

			return true;
		}


		void AsioTcpServer::Accept()
		{
			auto loop = EventLoopThreadPool::instance().GetNextLoop();
			auto handler = new AsioServerHandler(loop, msgqueue_);
			if (handler)
			{
				//handler->SetMsgHeaderProtocal(headprotol_);
				//handler->SetUserData(GetUserdata());
#if 0
				acceptorptr_->async_accept(handler->Socket(), 
					boost::bind(&AsioTcpServer::AsyncAcceptCallback, this, handler, asio::placeholders::error));
#endif 
				acceptorptr_->async_accept(handler->Socket(),
					[this,handler](const std::error_code& error)
					{
						if (!error)
						{
							AsyncAcceptCallback(handler);
						}
					});
			}

		}

		void AsioTcpServer::AsyncAcceptCallback(AsioServerHandler* handler)
		{

			auto& s = handler->Socket();
			uint64_t handle = HandlerManager::instance().AllocateHandlerId();
			handler->SetHandler(handle);
			handler->SetMsgHeaderProtocal(headprotol_);
			handler->SetUserData(GetUserdata());
			HandlerManager::instance().LinkHandler(handler);
			handler->OnAccept();
			max_connect_++;
#ifdef  TOM_ TOM_NET_DEBUG
			std::thread::id tid = std::this_thread::get_id();
			uint32_t hid = handler->GetHandler();
			printf("AsioTcpServer AsyncAcceptCallback , tid %d , handle %d \n", tid,hid);
#endif 
			Accept();
		}

		AsioEventLoop* AsioTcpServer::GetAcceptLoop()
		{
			return accept_thread_loop_.loop();
		}

		void AsioTcpServer::Stop()
		{

		}

	}
}