#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "base/pb_dispatcher.h"
#include "login.pb.h"
#include "main_option.h"

namespace tom {
class Buffer;
namespace container {
class Container;
}  // namespace container
namespace net {
class IMessageQueue;
struct NetContext;
}  // namespace net
}  // namespace gamesh
namespace cmd_proto {
class CommonMessage;
}  // namespace cmd_proto

class Network 
{
 public:
    Network();
    virtual ~Network();

    bool Init(nett::nett_conf* conf);
   
    uint64_t TcpServer(const std::string& ip, int port);
    bool TcpClient(const std::string& ip, int port, void* ud = nullptr);
    void Update();
    int32_t StopServer();

    // 发送消息给客户端
    bool SendMessage(uint32_t net_handler_id, const google::protobuf::Message& msg);
    void RegisterProtoCb();

public:
   std::vector<uint32_t> report;
   int onconnect = 0;
   int onclose = 0;
   bool isstart = false;
   nett::nett_conf* conf_;
   uint64_t server = 0;;
 public:

    void ProcessNetEvent(const std::shared_ptr<tom::Buffer>& event);
    void OnAccept(tom::net::NetContext* ctx);
    void OnClose(tom::net::NetContext* ctx);
    void OnConnected(tom::net::NetContext* ctx);
    void OnReConnected(tom::net::NetContext* ctx);


    void SendEcho(uint32_t handle);
    void OnTestEcho(uint32_t handle, void* ud, const std::shared_ptr<Tom::TestEcho>& message);
    void SendMsicMsg(uint32_t handle);
    //void OnNetMsg(tom::net::NetContext* ctx, const std::shared_ptr<cmd_proto::CommonMessage>& msg);



    tom::net::IMessageQueue* net_event_queue_ = nullptr;
    tom::pb::ProtobufDispatcher dispatcher_;
    std::string mod_ = "";
    bool isecho = false;
};
