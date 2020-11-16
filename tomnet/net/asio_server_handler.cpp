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
		//channel_.reset();
		channel_ = nullptr;
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

		auto fa = [this](){
			channel_->Close(handler_);
		};
		loop_->RunInIoService(std::move(fa));

		// 回收资源

		auto fb = [this](){
			uint64_t handle = GetHandler();
			loop_->RemoveHandler(handle);
		};

		loop_->RunInIoService(std::move(fb));

		return 0;
	}

	int32_t AsioServerHandler::OnAccept()
	{
		handler_ = GetHandler();
		NetContext context;
		context.handler_ = handler_;
		context.evetype_ = EVENT_ACCEPT;
		context.ud_ = nullptr;
		auto packet = std::make_shared<tom::Buffer>();
		packet->append(static_cast<const void*>(&context), sizeof(NetContext));
		if (msgqueue_)
		{
			msgqueue_->PushMessage(packet);
		}
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

	int32_t AsioServerHandler::SendPacket(const std::shared_ptr<tom::Buffer>& pPacket, uint16_t size)
	{
		int32_t ret = channel_->SendPacket(pPacket, size);
		return ret;
	}

	void AsioServerHandler::CloseLink(uint32_t handle)
	{
		channel_->Close(handle);
		//HandlerManager::instance().LinkDown(handler_);
	}
	
	void AsioServerHandler::FreePackage(const std::shared_ptr<tom::Buffer>& package)
	{
		channel_->FreePackage(package);
	}

	int32_t AsioServerHandler::LinkReady()
	{
		auto fn = [this]() {
			channel_->SetHandler(handler_);
			channel_->Start();
		};
		
		assert(loop_ != nullptr);
		loop_->RunInIoService(std::move(fn));
		return 0;
	}

}
}