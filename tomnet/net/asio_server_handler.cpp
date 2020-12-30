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
		channel_ = std::make_shared<AsioChannel>(loop_, 1024);
		channel_->SetMessageCb(std::bind(&AsioServerHandler::OnRecvMessage, this, std::placeholders::_1));
		channel_->SetErrorCb(std::bind(&AsioServerHandler::OnError, this, std::placeholders::_1));
		channel_->SetCloseCb(std::bind(&AsioServerHandler::OnDisConnected, this));
		channel_->SetWriteErrorCb(std::bind(&AsioServerHandler::OnWriteError, this, std::placeholders::_1));
	}

	AsioServerHandler::~AsioServerHandler()
	{
		#ifdef DEBUG_NET
			printf("%s:%s\n",__FILE__, __FUNCTION__);
		#endif 
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

		// ==================
		// 回调函数存在直接调用它
		if(messagecb_)
		{
			messagecb_(packet);
		}
		else if (msgqueue_)
		{
			msgqueue_->PushMessage(packet);
		}
		auto self = shared_from_this();
		channel_->RegisterHandler(self);
		return 0;
	}

	int32_t AsioServerHandler::OnDisConnected()
	{
		#ifdef DEBUG_NET
		printf("%s:%s:%d\n",__FILE__, __FUNCTION__,__LINE__);
		#endif 
		NetContext context;
		context.handler_ = handler_;
		context.evetype_ = EVENT_CLOSE;
		context.ud_ = GetUserdata();

		auto packet = std::make_shared<tom::Buffer>();
		packet->append(static_cast<const void*>(&context), sizeof(NetContext));
		// ==================
		// 回调函数存在直接调用它
		if(messagecb_)
		{
			messagecb_(packet);
		}
		else if (msgqueue_)
		{
			msgqueue_->PushMessage(packet);
		}
		OnError(0);  // 服务端手动对端断开连接的消息,销毁连接
		return 0;
	}


	int32_t AsioServerHandler::OnRecvMessage(const std::shared_ptr<tom::Buffer>& data)
	{
		NetContext* context = (NetContext*)data->peek();
		context->headerprotocal_ = GetMsgHeaderProtocal();
		context->ud_ = GetUserdata();

		// ==================
		// 回调函数存在直接调用它
		if(messagecb_)
		{
			messagecb_(data);
		}
		else if (msgqueue_)
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
		#ifdef DEBUG_NET
		printf("%s:%s\n",__FILE__, __FUNCTION__);
		#endif

		auto self = shared_from_this();
		auto fa = [self](){
			auto handler = std::dynamic_pointer_cast<AsioServerHandler>(self);
			handler->channel_->CloseSocket();
		};
		loop_->RunInIoService(fa);

		uint64_t handle = GetHandler();
		auto fb = [self, handle](){
			auto handler = std::dynamic_pointer_cast<AsioServerHandler>(self);
			handler->loop_->RemoveHandler(handle);
		};
		loop_->RunInIoService(fb);

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

	int32_t AsioServerHandler::SendPacket(const char* data, uint32_t size)
	{
		int32_t ret = channel_->SendPacket(data, size);
		return ret;
	}

	int32_t AsioServerHandler::SendPacket(const std::shared_ptr<tom::Buffer>& pPacket, uint32_t size)
	{
		int32_t ret = channel_->SendPacket(pPacket, size);
		return ret;
	}

	void AsioServerHandler::CloseLink()
	{
		// 服务器主动断开时在向上通知EVENT_CLOSE事件
		#ifdef DEBUG_NET
		printf("%s:%s:%d\n",__FILE__, __FUNCTION__,__LINE__);
		#endif
		NetContext context;
		context.handler_ = handler_;
		context.evetype_ = EVENT_CLOSE;
		context.ud_ = GetUserdata();

		auto packet = std::make_shared<tom::Buffer>();
		packet->append(static_cast<const void*>(&context), sizeof(NetContext));
		// ==================
		// 回调函数存在直接调用它
		if(messagecb_)
		{
			messagecb_(packet);
		}
		else if (msgqueue_)
		{
			msgqueue_->PushMessage(packet);
		}

		auto self = shared_from_this();
		auto fa = [self](){
			auto handler = std::dynamic_pointer_cast<AsioServerHandler>(self);
			handler->channel_->CloseSocket();
		};
		loop_->RunInIoService(fa);

		uint64_t handle = GetHandler();
		auto fb = [self, handle](){
			auto handler = std::dynamic_pointer_cast<AsioServerHandler>(self);
			handler->loop_->RemoveHandler(handle);
		};
		loop_->RunInIoService(fb);

	}

	int32_t AsioServerHandler::LinkReady()
	{
		channel_->SetHandler(handler_);
		channel_->Start();
		return 0;
	}

	void AsioServerHandler::CleanChannelCallback()
	{
		channel_->CleanCallback();
	}
	
	void AsioServerHandler::OnWriteError(int32_t error)
	{
		auto self = shared_from_this();
		auto fa = [self](){
			auto handler = std::dynamic_pointer_cast<AsioServerHandler>(self);
			handler->channel_->CloseSocket();
		};
		loop_->RunInIoService(fa);

		uint64_t handle = GetHandler();
		auto fb = [self, handle](){
			auto handler = std::dynamic_pointer_cast<AsioServerHandler>(self);
			handler->loop_->RemoveHandler(handle);
		};
		loop_->RunInIoService(fb);
	}

	bool AsioServerHandler::PostPacketToUpstream(const std::shared_ptr<tom::Buffer>& packet)
	{
		bool ret = true;
		if(messagecb_)
		{
			messagecb_(packet);
		}
		else if (msgqueue_)
		{
			msgqueue_->PushMessage(packet);
		}
		else
		{
			printf("TomNet AsioServerHandler PostUpstreamPacket Error Handle %d ", GetHandler());
			ret = false;
		}
		return ret;
	}

}
}