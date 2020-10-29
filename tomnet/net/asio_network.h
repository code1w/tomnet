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
		AsioTcpServer* tcpserver = new AsioTcpServer(loop, *queue, accepc_count);
		tcpserver->SetReuseAddr(true);
		tcpserver->SetMsgHeaderProtocal(headprotol);
		tcpserver->SetUserData(ud);
		if (!tcpserver->Start(address, port))
		{
			delete tcpserver;
			return 0;
		}

#ifdef  TOM_NET_TRAFFIC
		NetworkTraffic::trafmsgq_ = *queue;
		NetworkTraffic::instance().CreateTimer(&tcpserver->GetAcceptLoop()->io_service());
#endif 
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

		uint32_t hid = HandlerManager::instance().AllocateHandlerId();
		auto loop = EventLoopThreadPool::instance().GetNextLoopWithHash(hid);
		AsioClientHandler* handler = new AsioClientHandler(loop, *queue, hid);
		handler->SetMsgHeaderProtocal(headprotol);
		handler->SetUserData(ud);
		HandlerManager::instance().LinkHandler(handler);
		if (handler)
		{
			handler->AsyncConnect(address, port);
		}
		return hid;
	}

	static int32_t Asio_SendPacket(uint32_t handleid, const char* data, uint16_t size)
	{
		if(size > MAX_PACKET_SIZE)
		{
			return -1;
		}

		auto handler = HandlerManager::instance().FectHandler(handleid);
		if(handler)
		{
			bool ret = handler->SendPacket(data ,size);
			return ret;
		} 

		std::cout << "FectHandler Error " << handleid << std::endl;
		return -1;

	}

	static int32_t Asio_SendBuffer(uint32_t handleid, const tom::Buffer& b1, const tom::Buffer& b2)
	{

		if(b1.readableBytes() + b2.readableBytes() > MAX_PACKET_SIZE)
		{
			return -1;
		}

		tom::Buffer buf;
		buf.ensureWritableBytes(b1.readableBytes() + b2.readableBytes());
		buf.append(b1.peek(), b1.readableBytes());
		buf.append(b2.peek(), b2.readableBytes());

		auto handler = HandlerManager::instance().FectHandler(handleid);
		if(handler)
		{
			bool ret = handler->SendPacket(buf.peek() ,buf.readableBytes());
			return ret;
		} 

		std::cout << "FectHandler Error " << handleid << std::endl;
		return -1;

	}

	static int32_t Asio_InitLink(void* handle)
	{
		return 0;
	}

	static int32_t Asio_CloseLink(uint32_t handle)
	{
		auto handler = HandlerManager::instance().FectHandler(handle);
		if (handler)
		{
			handler->CloseLink(handle);
		}
		return 0;
	}

	static int32_t Asio_SetUserData(uint32_t handle, void* ud)
	{
		auto handler = HandlerManager::instance().FectHandler(handle);
		if (handler)
		{
			handler->SetUserData(ud);
		}
		return 0;
	}

	static MsgHeaderProtocal Asio_GetLinkMsgHeaderProtocal(uint32_t handle)
	{
		auto handler = HandlerManager::instance().FectHandler(handle);
		if (handler)
		{
			return handler->GetMsgHeaderProtocal();
		}
		return nametype;
	}



}
}
	
#endif  //  TOM_NET_ASIO_NETWORK
