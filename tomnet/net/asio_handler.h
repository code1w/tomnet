#ifndef  TOM_NET_ASIO_HANDLER_H
#define  TOM_NET_ASIO_HANDLER_H

#include "tomnet.h"
#include "base/buffer.h"
#include <memory>

namespace tomnet {
namespace net {
class AsiokHandler {
protected:
    uint64_t handler_;
    void* ud_;
    MsgHeaderProtocal headproto_ = nametype;

public:
    AsiokHandler() : ud_(nullptr), handler_(-1) {}
    virtual ~AsiokHandler() {}
    virtual int32_t OnDisConnected() = 0;
    virtual int32_t OnRecvMessage(const std::shared_ptr<tom::Buffer>& pPacket) = 0;
    virtual int32_t OnError(int32_t error) = 0;
    virtual int32_t OnConnected(uint32_t) = 0;
    virtual int32_t SendPacket(const char* data, uint16_t size) = 0;
    virtual void CloseLink(uint32_t) = 0;

    void SetHandler(uint64_t handle) { handler_ = handle; }
    uint64_t GetHandler() { return handler_; }
    void* GetUserdata() const { return ud_; }
    void SetUserData(void* ud) { ud_ = ud; }
    void SetMsgHeaderProtocal(MsgHeaderProtocal type) { headproto_ = type;}
    MsgHeaderProtocal GetMsgHeaderProtocal(){return headproto_;}
};
}}
#endif //  TOM_NET_ASIO_HANDLER_H