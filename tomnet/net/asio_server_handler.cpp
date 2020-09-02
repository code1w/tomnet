#include "asio_server_handler.h"
#include "tomnet.h"
#include "asio_channel.h"
#include "asio_event_loop.h"
#include "event_loop_thread_pool.h"
#include "handler_manager.h"
#include "base/buffer.h"
#include "asio/asio.hpp"

namespace tom {
namespace net {
	AsioServerHandler::AsioServerHandler(AsioEventLoop* loop, IMessageQueue* queue)
		:msgqueue_(queue)
		, loop_(loop)
	{
		channel_.reset(new AsioChannel(loop_, 1024));
		channel_->SetMessageCb(std::bind(&AsioServerHandler::OnRecvMessage, this, std::placeholders::_1));
		channel_->SetErrorCb(std::bind(&AsioServerHandler::OnError, this, std::placeholders::_1));
		channel_->SetCloseCb(std::bind(&AsioServerHandler::OnDisConnected, this));
	}

	AsioServerHandler::~AsioServerHandler()
	{
#ifdef  TOM_ TOM_NET_TRAFFIC
			printf("~AsioServerHandler()");
#endif
		channel_.reset();
	}

	int32_t AsioServerHandler::OnDisConnected()
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
		HandlerManager::instance().LinkDown(handler_);
		return 0;
	}

	int32_t AsioServerHandler::OnAccept()
	{
#ifdef  TOM_ TOM_NET_DEBUG
		std::thread::id tid = std::this_thread::get_id();
		//printf("AsioServerHandler OnAccept , tid %d \n", tid);
#endif
		handler_ = GetHandler();

		auto fn = [this]() {

			channel_->SetHandler(handler_);
			channel_->Start();
			NetContext context;
			context.handler_ = handler_;
			context.evetype_ = EVENT_ACCEPT;
			context.ud_ = GetUserdata();

			auto packet = std::make_shared<tom::Buffer>();
			packet->append(static_cast<const void*>(&context), sizeof(NetContext));
			if (msgqueue_)
			{
				msgqueue_->PushMessage(packet);
			}

#ifdef  TOM_ TOM_NET_DEBUG
			std::thread::id tid = std::this_thread::get_id();
			//printf("AsioServerHandler Channel Start , tid %d \n", tid);
#endif
		};

		assert(loop_ !=nullptr);
		loop_->RunInIoService(std::move(fn));

		return 0;
	}

	int32_t AsioServerHandler::OnRecvMessage(const std::shared_ptr<tom::Buffer>& data)
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

	int32_t AsioServerHandler::OnSendMessage(const std::shared_ptr<tom::Buffer>& msg)
	{
		return 0;
	}

	int32_t AsioServerHandler::OnError(int32_t error)
	{
		return 0;
	}

	void AsioServerHandler::Start()
	{

	}


	int32_t AsioServerHandler::OnConnected(uint32_t error)
	{
		return 0;
	}

	asio::ip::tcp::socket& AsioServerHandler::Socket()
	{
		return channel_->Socket();
	}

	int32_t AsioServerHandler::SendPacket(const char* data, uint16_t size)
	{
		int32_t ret = channel_->SendPacket(data, size);
		return ret;
	}

	void AsioServerHandler::CloseLink(uint32_t handle)
	{
		channel_->Close(handle);
		HandlerManager::instance().LinkDown(handler_);
	}

}
}