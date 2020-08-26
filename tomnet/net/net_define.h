#ifndef  NET_DEFINE_H
#define  NET_DEFINE_H
#include <functional>
#include <memory>

#include "tomnet.h"
#if (defined(WIN32) || defined(WIN64))
#define usleep(us) Sleep((us) / 1000)
#endif
#define MAXACCEPTPOOl 5000

namespace tom {
class Buffer;
namespace net {
class AsioServerHandler;
class AsioClientHandler;
class AsioChannel;
typedef std::function<void(const std::shared_ptr<tom::Buffer>&)> NetBufferCallback;
typedef std::function<void(int32_t)> NetErrorCallback;
typedef std::function<void(int32_t)> ConnectedCallback;
typedef std::function<void()> NetCloseCallback;
typedef std::shared_ptr<AsioChannel> AsioChannelPtr;
typedef std::shared_ptr<AsioServerHandler> AsioServerHandlerPtr;
typedef std::shared_ptr<AsioClientHandler> AsioClientHandlerPtr;
typedef std::shared_ptr<MessageHeader> HeaderPtr;
}  // namespace net
}  // namespace tom
#endif  //  NET_DEFINE_H
