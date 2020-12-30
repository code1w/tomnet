#ifndef  NET_ASIO_CLIENT_HANDLER_H
#define  NET_ASIO_CLIENT_HANDLER_H

#include "tomnet.h"
#include "asio_channel.h"
#include "asio_handler.h"
#include "asio/asio.hpp"
#include "base/tomnet_malloc.h"
#include <memory>
namespace tom {
class Buffer;
namespace net {
	class AsioEventLoop;
	class AsioClientHandler : public AsiokHandler
	{
	private:
		asio::steady_timer timer_;

		IMessageQueue* msgqueue_ = nullptr;
		AsioChannelPtr channel_;
		AsioEventLoop* loop_;
		std::string remoteip_;
		uint16_t remoteport_;

	public:
		AsioClientHandler(AsioEventLoop* loop, IMessageQueue* queue, uint32_t handle);
		AsioClientHandler(AsioEventLoop* loop, uint32_t handle);
		~AsioClientHandler();

	public:
		 int32_t OnAccept();
		 int32_t OnRecvMessage(const std::shared_ptr<tom::Buffer>& msg);
		 int32_t OnSendMessage(const std::shared_ptr<tom::Buffer>& msg);
		 int32_t OnError(int32_t error);
		 void OnWriteError(int32_t error);
		 int32_t OnConnected(uint32_t);
		 int32_t OnDisConnected();
		 int32_t OnReConnected(uint32_t);
		 int32_t SendPacket(const char* data, uint32_t size);
		 int32_t SendPacket(const std::shared_ptr<tom::Buffer>& pPacket, uint32_t size);
		 void    CloseLink();
		 void  SetHandler(uint32_t handler);
		uint64_t AsyncConnect(const char* addr, uint16_t port);
		void CleanChannelCallback();
		bool PostPacketToUpstream(const std::shared_ptr<tom::Buffer>& packet);

	};
}
}

#endif //  TOM_NET_ASIO_CLIENT_HANDLER_H