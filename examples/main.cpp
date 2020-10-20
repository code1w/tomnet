
#ifdef WIN32
#include <winsock2.h>  // for ntohl()
#else
#include <arpa/inet.h>  // for ntohl()
#endif  // _WIN32

#include "net/tomnet.h"
#include "base/buffer.h"
#include "base/any.h"  
#include "base/pb_dispatcher.h"
#include "base/pb_message_helper.h"

#include "net_msg_test_help.h"

#include "tellist.pb.h"
#include "asio/asio.hpp"

#include "net/protobuf-codec.h"
#include "net/default-network-protocol.h"
#include "net/default-packet-header.h"
#include "net/protobuf-networking.h"

#ifndef WIN32
#include <sys/time.h>
#include <sys/resource.h>
#endif

#include <functional>
#include <thread>
#include "login.pb.h"

using namespace tom::net;

#define RLIMIT_NOFILE 1000000
namespace net_test
{

std::string style = "";

asio::io_service io_service_;
tom::net::IMessageQueue* Q_ = nullptr;
tom::pb::ProtobufDispatcher gDispatcher_;
size_t total_count = 1;
std::vector<uint32_t> handles_;

}

using namespace net_test;

void OnRecvPacket(uint32_t handle, void* ud, const std::shared_ptr<tom::Buffer>& packet, tom::net::MsgHeaderProtocal headerprotocal = tom::net::nametype) 
{
    std::size_t qsize = Q_->Size();
    {
        auto header = std::make_shared<tom::net::MessageHeader>();
        auto msg = tom::pb::decode(packet, header, headerprotocal);
        gDispatcher_.onMessage(handle, ud, msg);
    }

}

int ProcessNetPackect(tom::net::IMessageQueue* msg_queue)
{
    
    int nProcessPacketCount = 0;
    auto packet = msg_queue->PopMessage();

    if (packet)
    {
      tom::net::NetContext* context =
          (tom::net::NetContext*)packet->peek();
      packet->retrieve(sizeof(tom::net::NetContext));
        switch (context->evetype_)
        {
        case EVENT_ACCEPT:
        {
            OnAccept(context->handler_, context->ud_, packet);
        }
        break;

        case EVENT_CONNECT_SUCCESS:
        {
            OnConnected(context->handler_, context->ud_ ,packet);
        }
        break;
        case EVENT_RECONNECTED:
        {
            OnReConnected(context->handler_,context->ud_, packet);
        }

        break;
        case EVENT_CLOSE:
            OnClose(context->handler_, context->ud_,packet);
            break;
        case EVENT_CONNECT_FAIL:
            OnConnectFail(context->handler_,context->ud_, packet);
        case EVENT_ERROR:
            break;
        case EVENT_NETMSG:
        {
            OnRecvPacket(context->handler_, context->ud_, packet, context->headerprotocal_);
        }
        break;
        default:
            break;
        }

        nProcessPacketCount++;
    }
    return nProcessPacketCount;
}

/// @brief 
/// 
/// 



void Update()
{
    while (true)
    {
        io_service_.run();
        while(Q_ && Q_->Size() > 0)
        {
            ProcessNetPackect(Q_);
        }

        if(style == "client")
        {
            for (auto handle : handles_)
            {
                //SendInfoList(handle);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }

        if(style == "server")
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }


    }
}


int usage() {
    std::cerr << "Usage: net_test.exe client <host> <port> <threads>"
        " <totalcount> <sessions>\n";
    std::cerr << "Usage: net_test.exe server <address> <port> <threads>"
        " <totalcount>\n";
    return 1;
}




#ifdef WIN32
void change_limit() {
}
#else
void change_limit() {
    rlimit rlim;
    if (getrlimit(RLIMIT_NOFILE, &rlim) < 0) {
        std::cout << "getrlimit failed, error: " << strerror(errno) << "\n";
        return;
    }
    if (rlim.rlim_cur < rlim.rlim_max) {
        auto old_cur = rlim.rlim_cur;
        rlim.rlim_cur = rlim.rlim_max;
        if (setrlimit(RLIMIT_NOFILE, &rlim) < 0) {
            std::cout << "setrlimit failed, error: " << strerror(errno) <<
                " " << std::to_string(old_cur) + " to " <<
                std::to_string(rlim.rlim_max) << "\n";
        }
        else {
            std::cout << "setrlimit success: " << std::to_string(old_cur) <<
                " to " << std::to_string(rlim.rlim_max) << "\n";
        }
    }
}
#endif

void TestAny()
{
    tom::Buffer* buf(new tom::Buffer());
    tom::Any any(buf);
    tom::Buffer* b = tom::any_cast<tom::Buffer*>(any);
    assert(buf == b);
    delete buf;
}

void TestTypeId()
{
	Tom::ReqLogin req;
	req.set_account("zxb-1");
	req.set_passward("1234546");
	auto tname = typeid(Tom::ReqLogin).raw_name();
    tname = typeid(Tom::ReqLogin).name();
    size_t hcode = typeid(Tom::ReqLogin).hash_code();
    hcode = typeid(Tom::ReqLogin).hash_code();

    auto ptr = std::make_shared<Tom::ReqLogin>();
    tname = typeid(ptr.get()).name();
    tname = typeid(*ptr).name();
}


void TestProtocol()
{
       
    INetWorkProtocol<google::protobuf::Message>* protocol  = 
        new DefaultNetWorkProtocol(new DefaultPacketHeader(), new ProtobufCodec());
	Tom::ReqLogin req;
	req.set_account("zxb-1");
	req.set_passward("1234546");

    tom::Buffer buffer;
    protocol->PackNetPacket(req, buffer);

    std::shared_ptr<tom::Buffer> binary = std::make_shared<tom::Buffer>();
    binary->append(buffer.peek(), buffer.readableBytes());
    auto msg = protocol->UnPackNetPacket(binary);

    TestAny();

}

// s 127.0.0.1 8888 4 1
int main(int argc, char** argv)
{
    TestProtocol();
    TestTypeId();
    if (argc < 2)
    {
        return usage();
    }
    if (!strcmp(argv[1], "client") && argc != 7)
    {
        return usage();
    }

    if (!strcmp(argv[1], "server") && argc != 6)
    {
        return usage();
    }

    if (!strcmp(argv[1], "client"))
    {
        style = "c";
        const char* host = argv[2];
        const char* port = argv[3];
        int thread_count = atoi(argv[4]);
        uint32_t total_count = atoi(argv[5]); // 一个包长度 K
        size_t session_count = atoi(argv[6]);
        client(host, port, thread_count, total_count, session_count);

    }
    else if (!strcmp(argv[1], "server")) {
        style = "s";
        const char* host = argv[2];
        const char* port = argv[3];
        int thread_count = atoi(argv[4]);
        uint32_t total_count = atoi(argv[5]); // 包长 K
        server(host, port, thread_count,total_count);
    }
    else {
        return usage();
    }

    INetWorking<google::protobuf::Message> pbnet;
    RegisterCb();
    Update();

    return 0;

}