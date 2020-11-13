#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "net/pb_dispatcher.h"


namespace tom {

}  // namespace container
namespace net {

class Network 
{
 public:
    Network();
    virtual ~Network();

    bool Init(int nettherad = 1);
   
    bool TcpServer(const std::string& ip, int port);
    bool TcpClient(const std::string& ip, int port, void* ud = nullptr);
    void Update();

    // 发送消息给客户端
    bool SendMessage(uint32_t net_handler_id, const google::protobuf::Message& msg);

 private:

    void ProcessNetEvent(const std::shared_ptr<gamesh::Buffer>& event);
    void OnAccept(gamesh::net::NetContext* ctx);
    void OnClose(gamesh::net::NetContext* ctx);
    void OnConnected(gamesh::net::NetContext* ctx);
    void OnReConnected(gamesh::net::NetContext* ctx);

    //void OnNetMsg(gamesh::net::NetContext* ctx, const std::shared_ptr<cmd_proto::CommonMessage>& msg);


    gamesh::net::IMessageQueue* net_event_queue_ = nullptr;
    gamesh::pb::ProtobufDispatcher dispatcher_;
};
}}