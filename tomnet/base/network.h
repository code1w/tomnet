#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "pb_dispatcher.h"


namespace tom {

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

    void ProcessNetEvent(const std::shared_ptr<tom::Buffer>& event);
    void OnAccept(tom::net::NetContext* ctx);
    void OnClose(tom::net::NetContext* ctx);
    void OnConnected(tom::net::NetContext* ctx);
    void OnReConnected(tom::net::NetContext* ctx);

    //void OnNetMsg(tom::net::NetContext* ctx, const std::shared_ptr<cmd_proto::CommonMessage>& msg);


    tom::net::IMessageQueue* net_event_queue_ = nullptr;
    tom::pb::ProtobufDispatcher dispatcher_;
};
}}