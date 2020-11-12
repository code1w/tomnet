#include "tomnet.h"
#include "base/buffer.h"
#include "asio_client_handler.h"
#include "asio_event_loop.h"
#include "handler_manager.h"

//#include "boost/bind/bind.hpp"


#include <iostream>


#define CONNECT_TIME_OUT 200
namespace tom {
namespace net {

	AsioClientHandler::AsioClientHandler(AsioEventLoop* loop, IMessageQueue* queue, uint32_t handle)
		:loop_(loop)
		,timer_(loop->io_service())

		,msgqueue_(queue)
	{
		handler_ = handle;
		channel_.reset(new AsioChannel(loop_, 1024));
		channel_->SetMessageCb(std::bind(&AsioClientHandler::OnRecvMessage, this, std::placeholders::_1));
		channel_->SetErrorCb(std::bind(&AsioClientHandler::OnError, this, std::placeholders::_1));
		channel_->SetCloseCb(std::bind(&AsioClientHandler::OnDisConnected, this));
		channel_->SetConnnectedCb(std::bind(&AsioClientHandler::OnConnected, this, std::placeholders::_1));
		channel_->SetReConnnectedCb(std::bind(&AsioClientHandler::OnReConnected, this, std::placeholders::_1));
		channel_->SetHandler(handler_);
	}



	AsioClientHandler::~AsioClientHandler() 
	{
#ifdef  TOM_NET_TRAFFIC
		printf("~AsioClientHandler()");
#endif
		//channel_.reset();
	}


	uint64_t AsioClientHandler::AsyncConnect(const char* address, uint16_t port)
	{
		remoteip_ = std::string(address);
		remoteport_ = port;

		std::error_code ec;
		asio::ip::address_v4 addr(asio::ip::address_v4::from_string(address, ec));
		if (ec)
		{
			return 0;
		}


		channel_->Connect(remoteip_, remoteport_);
		return 0;

	}

	int32_t AsioClientHandler::OnDisConnected()
	{
		NetContext context;
		context.handler_ = handler_;
		context.evetype_ = EVENT_CLOSE;
		context.ud_ = GetUserdata();

		auto packet = std::make_shared<tom::Buffer>();
		packet->append(static_cast<const void*>(&context), sizeof(NetContext));
		if (msgqueue_)
		{
			msgqueue_->PushMessage(packet);
		}

		return 0;
	}

	int32_t AsioClientHandler::OnReConnected(uint32_t error)
	{
		NetContext context;
		context.handler_ = handler_;
		context.evetype_ = error;
		context.ud_ = GetUserdata();

		auto packet = std::make_shared<tom::Buffer>();
		packet->append(static_cast<const void*>(&context), sizeof(NetContext));
		if (msgqueue_)
		{
			msgqueue_->PushMessage(packet);
		}
		return 0;
	}

	int32_t AsioClientHandler::OnAccept()
	{
		return 0;
	}

	int32_t AsioClientHandler::OnRecvMessage(const std::shared_ptr<tom::Buffer>& data)
	{
		NetContext* context = (NetContext*)data->peek();
		context->headerprotocal_ = GetMsgHeaderProtocal();
		context->ud_ = GetUserdata();
		if (msgqueue_)
		{
			msgqueue_->PushMessage(data);
		}
		return 0;
	}

	int32_t AsioClientHandler::OnSendMessage(const std::shared_ptr<tom::Buffer>& msg)
	{
		return 0;
	}

	int32_t AsioClientHandler::OnError(int32_t error)
	{
		return 0;
	}

	int32_t AsioClientHandler::OnConnected(uint32_t error)
	{
		NetContext context;
		context.handler_ = handler_;
		context.evetype_ = error;
		context.ud_ = GetUserdata();

		auto packet = std::make_shared<tom::Buffer>();
		packet->append(static_cast<const void*>(&context), sizeof(NetContext));
		if (msgqueue_)
		{
			msgqueue_->PushMessage(packet);
		}
		return 0;
	}

	void  AsioClientHandler::SetHandler(uint32_t handler)
	{ 
		handler_ = handler; 
	}

	int32_t AsioClientHandler::SendPacket(const char* data, uint16_t size)
	{
		int32_t ret = channel_->SendPacket(data, size);
		return ret;
	}
	
	int32_t AsioClientHandler::SendPacket(const std::shared_ptr<tom::Buffer>& pPacket, uint16_t size)
	{
		int32_t ret = channel_->SendPacket(pPacket, size);
		return ret;
	}

	void AsioClientHandler::CloseLink(uint32_t handle)
	{
		channel_->Close(handle);
		//HandlerManager::instance().LinkDown(handler_);
	}

	void AsioClientHandler::FreePackage(const std::shared_ptr<tom::Buffer>& package)
	{
		channel_->FreePackage(package);
	}

}
}