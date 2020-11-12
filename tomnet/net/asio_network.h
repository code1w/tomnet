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

namespace tom
{
namespace net
{
	static int32_t Asio_InitNetwork(uint32_t pool_size)
	{
		HandlerManager::instance().Initialize(5000);
		EventLoopThreadPool::instance().Initialize(pool_size);
		EventLoopThreadPool::instance().Start(true);
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
		AsioTcpServer* tcpserver =  new AsioTcpServer(loop, *queue, accepc_count);
		tcpserver->SetReuseAddr(true);
		tcpserver->SetMsgHeaderProtocal(headprotol);
		tcpserver->SetUserData(ud);
		if (!tcpserver->Start(address, port))
		{
			tcpserver = nullptr;
			return 0;
		}

		return (uint64_t)tcpserver;
	}

	static int32_t Asio_CloseNetService(uint64_t server_ptr)
	{
		AsioTcpServer* tcpserver = (AsioTcpServer*)server_ptr;
		tcpserver->Stop();
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
		AsioClientHandler* handler = new AsioClientHandler(loop, *queue, handle);
		if(!handler)
		{
			return -1;
		}


		std::string ip(address);
		auto fn = [handle, handler,headprotol,ud, ip, port, loop](){
			//std::weak_ptr<AsioClientHandler> wptr(handler);
			loop->AddHandler(handle,handler);
			handler->SetMsgHeaderProtocal(headprotol);
			handler->SetUserData(ud);
			handler->AsyncConnect(ip.c_str(), port);
		};
		loop->RunInIoService(std::move(fn));
		return handle;
	}

	static int32_t Asio_SendPacket(uint32_t handle, const char* data, uint16_t size)
	{
		if(size > MAX_PACKET_SIZE)
		{
			return -1;
		}

		auto loop = EventLoopThreadPool::instance().GetNextLoopWithHash(handle);
		if(!loop)
		{
			return -1;
		}
		
		auto packet = std::make_shared<tom::Buffer>();
		packet->ensureWritableBytes(size);
		packet->append(data, size);
		

		auto fn = [handle, packet,size, loop](){
			auto handler = loop->FetchAsioHandler(handle);
			if(handler)
			{
				handler->SendPacket(packet ,size);
			}
		};
		loop->RunInIoService(std::move(fn));
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
		auto fn = [handle, loop](){
			auto handler = loop->FetchAsioHandler(handle);
			if (handler)
			{
				handler->CloseLink(handle);
			}
		};
		loop->RunInIoService(std::move(fn));
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
		#if 0
		auto handler = HandlerManager::instance().FectHandler(handle);
		if (handler)
		{
			return handler->GetMsgHeaderProtocal();
		}
		#endif 
		return nametype;
	}


	static void Asio_FreeNetPackage(uint32_t handle, const std::shared_ptr<tom::Buffer>& package)
	{
		auto loop = EventLoopThreadPool::instance().GetNextLoopWithHash(handle);
		if(!loop)
		{
			return;
		}
		auto fn = [handle, loop, package](){
			auto handler = loop->FetchAsioHandler(handle);
			if (handler)
			{
				handler->FreePackage(package);
			}
		};
		loop->RunInIoService(std::move(fn));
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
