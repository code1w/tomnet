#ifndef  NET_ASIO_NETWORK_H
#define  NET_ASIO_NETWORK_H

#include "tomnet.h"
#include "message_queue.h"
#include "asio_tcp_server.h"
#include "handler_manager.h"
#include "asio_client_handler.h"
#include "asio_event_loop.h"
#include "event_loop_thread_pool.h"
#include "network_traffic.h"
#include "asio_handler.h"
#include "dll_export.h"
#include "base/tomnet_malloc.h"
#include "tcp_server_manager.h"

namespace tom
{
namespace net
{
	static  int32_t Asio_InitNetwork(uint32_t pool_size)
	{
		HandlerManager::instance().Initialize(5000);
		EventLoopThreadPool::instance().Initialize(pool_size);
		auto isok = EventLoopThreadPool::instance().Start(true);
		if(isok)
		{

		}
		return 0;
	}

	static uint64_t Asio_StartNetService(const char* address, uint16_t port, IMessageQueue** queue, uint32_t ping_invent, uint32_t timeout, uint16_t accepc_count,MsgHeaderProtocal headprotol = nametype, void* ud = nullptr)
	{
		if (*queue == nullptr)
		{
			*queue = new MessageQueue(); 
		}

		if (queue == nullptr)
		{
			return 0;
		}

		auto loop = EventLoopThreadPool::instance().GetNextLoop();
		AsioTcpServer* tcpserver = new AsioTcpServer(loop, *queue, accepc_count);
		tcpserver->SetReuseAddr(true);
		tcpserver->SetMsgHeaderProtocal(headprotol);
		tcpserver->SetUserData(ud);
		if (!tcpserver->Start(address, port))
		{
			// TODO(zxb): Stop后在销毁
			delete tcpserver;
			return 0;
		}

		// 启动的服务添加到管理器
		std::string srvkey(address);
		srvkey = srvkey + ":" + std::to_string(port);
		TcpServerManager::instance().AddSomePortTcpServer(srvkey, (uint64_t)tcpserver);

		return (uint64_t)tcpserver;
	}

	static uint64_t Asio_StartNetServiceWithCb(const char* address, uint16_t port, const NetMessageCallback& cb, uint32_t ping_invent, uint32_t timeout, uint16_t accepc_count,MsgHeaderProtocal headprotol = nametype)
	{
		auto loop = EventLoopThreadPool::instance().GetNextLoop();
		AsioTcpServer* tcpserver = new AsioTcpServer(loop, accepc_count);
		tcpserver->SetReuseAddr(true);
		tcpserver->SetMsgHeaderProtocal(headprotol);
		tcpserver->SetMessageCb(cb);
		if (!tcpserver->Start(address, port))
		{
			// TODO(zxb): Stop后在销毁
			tcpserver->Stop();
			delete tcpserver;
			return 0;
		}

		// 启动的服务添加到管理器
		std::string srvkey(address);
		srvkey = srvkey + ":" + std::to_string(port);
		TcpServerManager::instance().AddSomePortTcpServer(srvkey, (uint64_t)tcpserver);

		return (uint64_t)tcpserver;
	}

	static int32_t Asio_CloseNetService(uint64_t server_ptr)
	{
		AsioTcpServer* tcpserver = (AsioTcpServer*)server_ptr;
		tcpserver->Stop();

		// 
		delete tcpserver;
		return 0;
	}

	static int32_t Asio_Connect(const char* address, uint16_t port, IMessageQueue** queue, uint32_t ping_invent, uint32_t timeout, MsgHeaderProtocal headprotol = nametype, void* ud = nullptr)
	{
		if (*queue == nullptr)
		{
			*queue = new MessageQueue();
		}

		uint32_t handle = HandlerManager::instance().AllocateHandlerId();
		auto loop = EventLoopThreadPool::instance().GetNextLoopWithHash(handle);
		std::shared_ptr<AsioClientHandler> handler =std::make_shared<AsioClientHandler>(loop, *queue, handle);
		if(!handler)
		{
			return -1;
		}

		std::string ip(address);
		auto fn = [handle, handler,headprotol,ud, ip, port, loop](){
			loop->AddHandler(handle,handler);
			handler->SetMsgHeaderProtocal(headprotol);
			handler->SetUserData(ud);
			handler->AsyncConnect(ip.c_str(), port);
		};
		loop->RunInIoService(std::move(fn));
		return handle;
	}

	static int32_t Asio_ConnectWithCb(const char* address, uint16_t port, const NetMessageCallback& cb, uint32_t ping_invent, uint32_t timeout, MsgHeaderProtocal headprotol = nametype, void* ud = nullptr)
	{
		uint32_t handle = HandlerManager::instance().AllocateHandlerId();
		auto loop = EventLoopThreadPool::instance().GetNextLoopWithHash(handle);
		std::shared_ptr<AsioClientHandler> handler =std::make_shared<AsioClientHandler>(loop,handle);
		if(!handler)
		{
			return -1;
		}

		handler->SetMessageCb(cb);

		std::string ip(address);
		auto fn = [handle, handler,headprotol,ud, ip, port, loop](){
			loop->AddHandler(handle,handler);
			handler->SetMsgHeaderProtocal(headprotol);
			handler->SetUserData(ud);
			handler->AsyncConnect(ip.c_str(), port);
		};
		loop->RunInIoService(std::move(fn));
		return handle;
	}

	static int32_t Asio_SendPacket(uint32_t handle, const char* data, uint32_t size)
	{
		if(size > MAX_PACKET_SIZE)
		{
			CloseLink(handle);
			return -1;
		}

		auto loop = EventLoopThreadPool::instance().GetNextLoopWithHash(handle);
		if(!loop)
		{
			return -1;
		}

		// TODO(zhangxiaobin): 如何减少内存分配次数,考虑复用buffer.
		auto packet = std::make_shared<tom::Buffer>();
		packet->ensureWritableBytes(size);
		packet->append(data, size);
		auto handler = loop->FetchAsioHandler(handle);
		if(handler)
		{
			auto fn = [handler,packet,size](){
				handler->SendPacket(packet ,size);
			};
			loop->RunInIoService(std::move(fn));

		}
		return 0;

	}

	static int32_t Asio_SendBuffer(uint32_t handle, const tom::Buffer& b1, const tom::Buffer& b2)
	{

		if(b1.readableBytes() + b2.readableBytes() > MAX_PACKET_SIZE)
		{
			return -1;
		}

		std::shared_ptr<tom::Buffer> buf = std::make_shared<tom::Buffer>();
		buf->ensureWritableBytes(b1.readableBytes() + b2.readableBytes());
		buf->append(b1.peek(), b1.readableBytes());
		buf->append(b2.peek(), b2.readableBytes());

		auto loop = EventLoopThreadPool::instance().GetNextLoopWithHash(handle);
		if(!loop)
		{
			return -1;
		}
		auto fn = [handle, loop, buf](){
			auto handler = loop->FetchAsioHandler(handle);
			if (handler)
			{
				handler->SendPacket(buf->peek() ,buf->readableBytes());
			}
		};
		loop->RunInIoService(std::move(fn));
		return 0;

	}

	static int32_t Asio_InitLink(void* handle)
	{
		return 0;
	}

	static int32_t Asio_CloseLink(uint32_t handle)
	{
		auto loop = EventLoopThreadPool::instance().GetNextLoopWithHash(handle);
		if(!loop)
		{
			return -1;
		}
		auto handler = loop->FetchAsioHandler(handle);
		if (handler)
		{
			handler->CloseLink();
		}
		return 0;
	}

	static int32_t Asio_SetUserData(uint32_t handle, void* ud)
	{
		auto loop = EventLoopThreadPool::instance().GetNextLoopWithHash(handle);
		if(!loop)
		{
			return -1;
		}
		auto fn = [handle, loop, ud](){
			auto handler = loop->FetchAsioHandler(handle);
			if (handler)
			{
				handler->SetUserData(ud);
			}
		};
		loop->RunInIoService(std::move(fn));

		return 0;
	}

	static MsgHeaderProtocal Asio_GetLinkMsgHeaderProtocal(uint32_t handle)
	{
		return nametype;
	}

	static int32_t Asio_LinkReady(uint32_t handle,void* ud)
	{
		auto loop = EventLoopThreadPool::instance().GetNextLoopWithHash(handle);
		if(!loop)
		{
			return -1;
		}


		auto fn = [handle, loop, ud](){
			auto handler = loop->FetchAsioHandler(handle);
			if(handler)
			{
				handler->SetUserData(ud);
				handler->LinkReady();
			}
		};
		loop->RunInIoService(std::move(fn));
		return 0;
	}



}
}
	
#endif  //  TOM_NET_ASIO_NETWORK
