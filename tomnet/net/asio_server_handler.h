#ifndef  TOM_NET_ASIO_SERVER_HANDLER_H
#define  TOM_NET_ASIO_SERVER_HANDLER_H
#include <memory>

#include "boost/asio.hpp"
#include "tomnet.h"
#include " TOM_NET_define.h"
#include "asio_handler.h"
namespace tomnet {
class Buffer;
namespace net {
class AsioEventLoop;
class AsioServerHandler : public AsiokHandler,
                          std::enable_shared_from_this<AsioServerHandler> {
   private:
    IMessageQueue* msgqueue_ = nullptr;
    AsioEventLoop* loop_ = nullptr;
    AsioChannelPtr channel_;

   public:
    AsioServerHandler(AsioEventLoop* loop, IMessageQueue* queue);
    ~AsioServerHandler();

   public:
    int32_t OnDisConnected();
    int32_t OnAccept();
    int32_t OnRecvMessage(const std::shared_ptr<tom::Buffer>& msg);
    int32_t OnSendMessage(const std::shared_ptr<tom::Buffer>& msg);
    int32_t OnError(int32_t error);
    int32_t OnConnected(uint32_t);
    int32_t SendPacket(const char* data, uint16_t size);
    void CloseLink(uint32_t handle);

   public:
    void Start();
    boost::asio::ip::tcp::socket& Socket();
};

}  // namespace net
}  // namespace tomnet
#endif  //  TOM_NET_ASIO_SERVER_HANDLER_H
