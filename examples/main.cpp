
#ifdef WIN32
#include <winsock2.h>  // for ntohl()
#else
#include <arpa/inet.h>  // for ntohl()
#endif  // _WIN32

#include "gamesh_net/net_base.h"
#include "common/buffer.h"
#include "common/codec/pb_dispatcher.h"
#include "common/pb_message_helper.h"

#include "net_msg_test_help.h"

#include "tellist.pb.h"

#ifndef WIN32
#include <sys/time.h>
#include <sys/resource.h>
#endif

#include <functional>
#include <thread>

#define RLIMIT_NOFILE 1000000

std::string style = "";


gamesh::net::IMessageQueue* Q_ = nullptr;
gamesh::pb::ProtobufDispatcher gDispatcher_;
size_t total_count = 1;
std::vector<uint32_t> handles_;


void OnRecvPacket(uint32_t handle, void* ud, const std::shared_ptr<gamesh::Buffer>& packet, gamesh::net::MsgHeaderProtocal headerprotocal = gamesh::net::nametype) 
{
    auto fn = [](int32_t id) -> std::string {
        return "bus.CommonMessage";
    };

    std::size_t qsize = Q_->Size();
    {
        auto header = std::make_shared<gamesh::net::MessageHeader>();
        auto msg = gamesh::pb::decode(packet, header, headerprotocal, fn);
        gDispatcher_.onMessage(handle, ud, header, msg);
    }

}

int ProcessNetPackect(gamesh::net::IMessageQueue* msg_queue)
{
    
    int nProcessPacketCount = 0;
    auto packet = msg_queue->PopMessage();

    if (packet)
    {
      gamesh::net::NetContext* context =
          (gamesh::net::NetContext*)packet->peek();
      packet->retrieve(sizeof(gamesh::net::NetContext));
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

void Update()
{
    while (true)
    {
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

int main(int argc, char** argv)
{
   // TestConcurrentqueue();
    //TestEcodeAndDecode();
    //TestEcodeAndDecodeDaobatuProto();
    //TestEcodeAndDecodeDaobatuProtoRegister();
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
        style = "client";
        const char* host = argv[2];
        const char* port = argv[3];
        int thread_count = atoi(argv[4]);
        uint32_t total_count = atoi(argv[5]); // 一个包长度 K
        size_t session_count = atoi(argv[6]);
        client(host, port, thread_count, total_count, session_count);

    }
    else if (!strcmp(argv[1], "server")) {
        style = "server";
        const char* host = argv[2];
        const char* port = argv[3];
        int thread_count = atoi(argv[4]);
        uint32_t total_count = atoi(argv[5]); // 包长 K
        server(host, port, thread_count,total_count);
    }
    else {
        return usage();
    }
    RegisterCb();
    Update();

    return 0;

}