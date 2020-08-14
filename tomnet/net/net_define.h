#ifndef  TOM_NET_ TOM_NET_DEFINE_H
#define  TOM_NET_ TOM_NET_DEFINE_H
#include <functional>
#include <memory>

#include "tomnet.h"
#if (defined(WIN32) || defined(WIN64))
#define usleep(us) Sleep((us) / 1000)
#endif
#define MAXACCEPTPOOl 5000

namespace tomnet {
class Buffer;
namespace net {
class AsioServerHandler;
class AsioClientHandler;
class AsioChannel;

typedef std::function<void(const std::shared_ptr<tom::Buffer>&)> NetBufferCallback;
typedef std::function<void(int32_t)> NetErrorCallback;
typedef std::function<void(int32_t)> ConnectedCallback;
typedef std::function<void()> NetCloseCallback;
typedef std::function<void(const std::shared_ptr<google::protobuf::Message>&)> NetPbMessageCallback;

typedef std::shared_ptr<AsioChannel> AsioChannelPtr;
typedef std::shared_ptr<AsioServerHandler> AsioServerHandlerPtr;
typedef std::shared_ptr<AsioClientHandler> AsioClientHandlerPtr;
}  // namespace net
}  // namespace tomnet
#endif  //  TOM_NET_ TOM_NET_DEFINE_H
