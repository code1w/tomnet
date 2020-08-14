#ifndef  TOM_NET_ASIO_CLIENT_HANDLER_H
#define  TOM_NET_ASIO_CLIENT_HANDLER_H

#include "tomnet.h"
#include "asio_channel.h"
#include "asio_handler.h"
#include "boost/asio.hpp"
#include <memory>
namespace tomnet {
class Buffer;
namespace net {
	class AsioEventLoop;
	class AsioClientHandler : public AsiokHandler, 
		std::enable_shared_from_this<AsioClientHandler>
	{
	private:
		boost::asio::deadline_timer timer_;

		IMessageQueue* msgqueue_ = nullptr;
		AsioChannelPtr channel_;
		AsioEventLoop* loop_;
		std::string remoteip_;
		uint16_t remoteport_;

	public:
		AsioClientHandler(AsioEventLoop* loop, IMessageQueue* queue, uint32_t handle);
		~AsioClientHandler();

	public:
		 int32_t OnAccept();
		 int32_t OnRecvMessage(const std::shared_ptr<tom::Buffer>& msg);
		 int32_t OnSendMessage(const std::shared_ptr<tom::Buffer>& msg);
		 int32_t OnError(int32_t error);
		 int32_t OnConnected(uint32_t);
		 int32_t OnDisConnected();
		 int32_t OnReConnected(uint32_t);
		 int32_t SendPacket(const char* data, uint16_t size);
		 void    CloseLink(uint32_t handle);
		 void  SetHandler(uint32_t handler);
		 int32_t OnRecvPbMessage(const std::shared_ptr<google::protobuf::Message>& msg);
		uint64_t AsyncConnect(const char* addr, uint16_t port);

	};

}
}

#endif //  TOM_NET_ASIO_CLIENT_HANDLER_H