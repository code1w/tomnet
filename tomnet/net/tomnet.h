#ifndef NET_TOMNET_BASE
#define NET_TOMNET_BASE

#include <memory>

#include "base/buffer.h"
#include "base/any.h"
#include "dll_export.h"

//#include <google/protobuf/message.h>

#ifdef  TOM_NET_EXPORTS
#define  TOM_NET_API __declspec(dllexport)
#else
#define  TOM_NET_API __declspec(dllimport)
#endif


#define MAX_PACKET_SIZE (1024 * 1024)  
#define INVALID_NETWORK_HANDLE (~0)
#define MAXFREENETPACKET 1024
#define MAXSYSTEMPACKETLEN 128 
#define MAXMSGHEADERLEN 256 
#define MAXMSGNAMELEN 128 
#define RECONNECTTIME 3000


#define VERIFIY_MASK_PROTOCAL (1)
#define VERIFIY_MASK_BODY (2)

namespace tom {
class Buffer;
}

namespace tom {
namespace net {

class NetworkPacket;

#define SYSTEM_MESSAGE_TYPE 0
#define EVENT_ACCEPT 1
#define EVENT_CLOSE 2
#define EVENT_CONNECT_SUCCESS 3
#define EVENT_CONNECT_FAIL 4
#define EVENT_ERROR 5
#define EVENT_PING 6
#define EVENT_PONG 7
#define EVENT_NETMSG 8
#define EVENT_OUTTIME 9
#define EVENT_RECONNECTED 10
#define EVENT_CALLRPC 11


#define  TOM_NET_ERROR_NOT_ENOUGH_MEMROY 0x87771000L
#define  TOM_NET_ERROR_HEADER_LENGTH 0x87771001L
#define  TOM_NET_ERROR_DECRYPT_LENGTH 0x87771002L
#define  TOM_NET_ERROR_PACKET_SPACE 0x87771003L
#define  TOM_NET_ERROR_PACKET_INVALID 0x87771004L

enum NetDriver {
    asio,  // boost asio tcp
    quic,
    kcp,
};

enum PacketType {
    network,
    system,
};

// 消息头部类型
enum MsgHeaderProtocal 
{
    nametype, // | 4字节 (消息长度) | 4字节 (类型) | 4字节 (pb消息名字) | 消息名字 | 消息内容 |
};

#pragma pack(push, 1)
struct MessageHeader {
    int32_t size_;
    union {
        struct {
            int32_t type_;
            int32_t namelen_;
            char name[MAXMSGNAMELEN];
        }type_name;
    }u;
};

struct NetContext {
    uint32_t handler_;
    uint8_t evetype_;
    MsgHeaderProtocal headerprotocal_;
    void* ud_;
};

#pragma pack(pop)


class IMsgHeader{
public:
    int32_t packetlen;
    int32_t packetop;
    bool encrypted;
    bool compressed;
    bool binary;
};


class IMsgCodec
{
public:
    virtual std::shared_ptr<tom::Buffer> GenerateBinaryMessage(IMsgHeader* , Any&) = 0;
    virtual std::shared_ptr<Any> GenerateMessage(IMsgHeader* ,std::shared_ptr<tom::Buffer>& ) = 0;
};


class INetWorkProtocal {
public:
    IMsgHeader* header = nullptr;
    IMsgCodec* codec = nullptr;
public:
    /*
    virtual uint32_t HeaderSize() = 0;
    virtual uint32_t PacketLength(const char* header) = 0;
    virtual uint32_t PacketFilter(const char* header) = 0;
    virtual uint32_t PacketSystem(uint8_t event, char* header) = 0;
    virtual uint32_t PacketFinial(char* header, uint32_t length) = 0;
    */
};

class IMessageQueue {
   public:
    virtual std::shared_ptr<tom::Buffer> PopMessage() = 0;
    virtual void PushMessage(const std::shared_ptr<tom::Buffer>&, bool front = false) = 0;
    virtual bool IsEmpty() = 0;
    virtual std::size_t Size() = 0;
};

 TOM_TOM_NET_DLL_DECL int32_t (*InitNetwork)(uint32_t threads);
 TOM_TOM_NET_DLL_C_DECL uint64_t (*StartNetService)(const char* address, uint16_t port, IMessageQueue** q, uint32_t , uint32_t , uint16_t, MsgHeaderProtocal, void*);
 TOM_TOM_NET_DLL_C_DECL int32_t (*CloseNetService)(uint64_t server_h);
 TOM_TOM_NET_DLL_C_DECL int32_t (*Connect)(const char* ip, uint16_t port,IMessageQueue** q,uint32_t , uint32_t , MsgHeaderProtocal, void*);
 TOM_TOM_NET_DLL_C_DECL int32_t (*SendPacket)(uint32_t handle, const char* data,uint16_t size);
 TOM_TOM_NET_DLL_C_DECL int32_t (*SendBuffer)(uint32_t handle, const tom::Buffer& b1,const tom::Buffer& b2);
 TOM_TOM_NET_DLL_C_DECL int32_t (*CloseLink)(uint32_t handle);
 TOM_TOM_NET_DLL_C_DECL int32_t (*SetUserData)(uint32_t handle, void* ud);
 TOM_TOM_NET_DLL_C_DECL MsgHeaderProtocal (*GetLinkMsgHeaderProtocal)(uint32_t handle);
 TOM_TOM_NET_DLL_C_DECL bool CreateNetwork(NetDriver);
 TOM_TOM_NET_DLL_C_DECL void DestroyNetwork();

}  // namespace net
}  // namespace tom

#endif
