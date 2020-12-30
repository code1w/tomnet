#ifndef  NET_ASIO_SERVER_HANDLER_H
#define  NET_ASIO_SERVER_HANDLER_H

#include <memory>
#include "asio/asio.hpp"
#include "tomnet.h"
#include "net_define.h"
#include "asio_handler.h"
#include "base/tomnet_malloc.h"
namespace tom {
class Buffer;
namespace net {
class AsioEventLoop;
class AsioServerHandler : public AsiokHandler
{
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
    void OnWriteError(int32_t error);
    int32_t OnConnected(uint32_t);
    int32_t SendPacket(const char* data, uint32_t size);
    int32_t SendPacket(const std::shared_ptr<tom::Buffer>& pPacket, uint32_t size);
    void CloseLink();
    int32_t LinkReady();
    void CleanChannelCallback();
    bool PostPacketToUpstream(const std::shared_ptr<tom::Buffer>& packet);
    
    

   public:
    void Start();
    asio::ip::tcp::socket& Socket();
};
}  // namespace net
}  // namespace tom
#endif  //  TOM_NET_ASIO_SERVER_HANDLER_H
