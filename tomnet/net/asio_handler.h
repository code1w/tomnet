#ifndef  NET_ASIO_HANDLER_H
#define  NET_ASIO_HANDLER_H

#include "tomnet.h"
#include "net_define.h"
#include "base/buffer.h"
#include <memory>

namespace tom {
namespace net {
class AsiokHandler : public std::enable_shared_from_this<AsiokHandler>{
protected:
    uint64_t handler_;
    void* ud_;
    MsgHeaderProtocal headproto_ = nametype;
    NetMessageCallback messagecb_ = nullptr;

public:
    AsiokHandler() : ud_(nullptr), handler_(-1) {}
    virtual ~AsiokHandler() {}
    virtual int32_t OnDisConnected() = 0;
    virtual int32_t OnRecvMessage(const std::shared_ptr<tom::Buffer>& pPacket) = 0;
    virtual int32_t OnError(int32_t error) = 0;
    virtual int32_t OnConnected(uint32_t) = 0;
    virtual int32_t SendPacket(const char* data, uint32_t size) = 0;
    virtual int32_t SendPacket(const std::shared_ptr<tom::Buffer>& pPacket, uint32_t size) = 0;
    virtual void CloseLink() = 0;
    virtual int32_t LinkReady(){return 0;};
    virtual void CleanChannelCallback() = 0;
    virtual bool PostPacketToUpstream(const std::shared_ptr<tom::Buffer>& packet) = 0;


    void SetHandler(uint64_t handle) { handler_ = handle; }
    uint64_t GetHandler() { return handler_; }
    void* GetUserdata() const { return ud_; }
    void SetUserData(void* ud) { ud_ = ud; }
    void SetMsgHeaderProtocal(MsgHeaderProtocal type) { headproto_ = type;}
    MsgHeaderProtocal GetMsgHeaderProtocal(){return headproto_;}
    void SetMessageCb(const NetMessageCallback& cb){messagecb_ = cb;}
};
}}
#endif //  TOM_NET_ASIO_HANDLER_H