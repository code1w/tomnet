#include "network.h"
#include "net/tomnet.h"
#include "base/pb_codec.h"
#include <iostream>
#include "main_option.h"

Network::Network()
{
}

Network::~Network()
{

}

bool Network::Init(nett::nett_conf* conf)
{
    conf_ = conf;
    bool isstart = false; 
    tom::net::CreateNetwork(tom::net::asio);
    tom::net::InitNetwork(conf_->iothread);
    tom::net::InitNetwork(conf_->iothread);
    tom::net::InitNetwork(conf_->iothread);
    RegisterProtoCb();
    return isstart;
}

uint64_t Network::TcpServer(const std::string& ip, int port)
{
    server = tom::net::StartNetService(ip.c_str(), port, &net_event_queue_, 10000, 250000, 100, tom::net::nametype, nullptr);
    if (server == 0)
    {
        //LOG_TRACE << "Network::TcpServer 失败! " << ip.c_str() << ":" << port;
        return server;
    }
    printf("TcpServer %s:%d\n",ip.c_str(), port);
    return server;
}

bool Network::TcpClient(const std::string& ip, int port, void* ud)
{
    auto ret =  tom::net::Connect(ip.c_str(), port, &net_event_queue_, 10000, 25000, tom::net::nametype,ud);
    if(ret < 0)
    {
        //LOG_TRACE << "Network::TcpClient 失败! " << ip.c_str() << ":" << port;
        return false;
    }
    return true;
}

void Network::Update()
{
    if (!net_event_queue_)
        return;


    auto event = net_event_queue_->PopMessage();
    while (event)
    {
        ProcessNetEvent(event);
        event = net_event_queue_->PopMessage();
    }

}

void Network::ProcessNetEvent(const std::shared_ptr<tom::Buffer>& event)
{
    
    tom::net::NetContext* ctx = (tom::net::NetContext*)event->peek();
    event->retrieve(sizeof(tom::net::NetContext));

    switch (ctx->evetype_)
    {
        case EVENT_ACCEPT:
        {
            OnAccept(ctx);
        }
        break;
        case EVENT_CLOSE :
        {
            OnClose(ctx);
        }
        break;
        case EVENT_CONNECT_SUCCESS:
        {
            OnConnected(ctx);
        }
        break;
        case EVENT_RECONNECTED:
        {
            OnReConnected(ctx);
        }
        break;
        case EVENT_NETMSG:
        {
            static auto header = std::make_shared<tom::net::MessageHeader>();
            auto msg = tom::pb::decode(event, header, tom::net::nametype);
            if (msg)
            {
                dispatcher_.onMessage(ctx->handler_, ctx->ud_, msg);
            }
            else
            {
                printf("%s netmsg decode error\n", __FUNCTION__);
            }
        }
        break;
        default:
        {
            static uint32_t reportidx = 1;
            printf("%s unknown event %d\n", __FUNCTION__, ctx->evetype_);
            if(report.size() <500)
            {
                report.push_back(reportidx);
                reportidx++;
            }
            else
            {
                reportidx = 1;
            }
            
        }
        break;
    }  // switch
}

void Network::OnAccept(tom::net::NetContext* ctx)
{
    const uint32_t net_handler_id = ctx->handler_;
    printf("%s : %s handler=%lu\n", __FILE__, __FUNCTION__, net_handler_id);
    tom::net::LinkReady(net_handler_id, nullptr);
    //tom::net::CloseLink(net_handler_id);
    StopServer();

}

void Network::OnClose(tom::net::NetContext* ctx)
{
    static int CloseTimes = 0;
    ++onclose;
    ++CloseTimes;
    //printf("%s : %s CloseTimes=%lu\n", __FILE__, __FUNCTION__, CloseTimes);
    const uint32_t net_handler_id = ctx->handler_;
    //printf("%s : %s handler=%lu\n", __FILE__, __FUNCTION__, net_handler_id);

    tom::net::SetUserData(net_handler_id, nullptr);
    if(mod_== "c")
    {
        //TcpClient("192.168.12.73", 8888 ,NULL);
    }
}


void Network::OnConnected(tom::net::NetContext* ctx)
{
    static int connecttimes = 0;
    ++connecttimes;
    const uint32_t net_handler_id = ctx->handler_;
    //printf("%s : %s handler=%lu\n", __FILE__, __FUNCTION__, net_handler_id);

    tom::net::SetUserData(net_handler_id, nullptr);
    if(conf_->echo > 0)
    {
        SendEcho(net_handler_id);
        //SendMsicMsg(net_handler_id);
    }
    if(conf_->dropclient > 0)
    {
        tom::net::CloseLink(net_handler_id);
        if(connecttimes >= conf_->tasks)
        {
            exit(0);
        }
    }

}

void Network::OnReConnected(tom::net::NetContext* ctx)
{
    const uint32_t net_handler_id = ctx->handler_;
    printf("%s : %s handler=%lu\n", __FILE__, __FUNCTION__, net_handler_id);

    tom::net::SetUserData(net_handler_id, nullptr);
    SendEcho(net_handler_id);
}


bool Network::SendMessage(uint32_t net_handler_id, const google::protobuf::Message& msg)
{
    tom::Buffer buf;
    tom::pb::encode(msg, &buf, tom::net::nametype);

    const uint32_t size = buf.readableBytes();
    tom::net::SendPacket(net_handler_id, buf.peek(), size);
    return true;
}

void Network::SendMsicMsg(uint32_t handle)
{
    tom::Buffer buf;
    buf.appendInt32(1);
     buf.appendInt32(8888);
    tom::net::SendPacket(handle, buf.peek(), buf.readableBytes());

}

void Network::SendEcho(uint32_t handle)
{
	static int32_t x = 1;
    static char * teststr = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";
    char index[128];
	sprintf(index, "%u", x);
	Tom::TestEcho req;
	req.set_index(index);
	req.set_msg(teststr);
	int64_t time = timenow();
	req.set_time(time);
    for(int i = 0 ; i < 100000; i++)
    {
        int x = i*i;
    }
    SendMessage(handle, req);

}
void Network::OnTestEcho(uint32_t handle, void* ud, const std::shared_ptr<Tom::TestEcho>& message)
{
/*
    printf("%s : %s handler=%lu\n", __FILE__, __FUNCTION__, handle);
    SendEcho(handle);
    if(conf_->mod == "s")
    {
        //tom::net::CloseLink(handle);
    }
*/
}
void Network::RegisterProtoCb()
{
	dispatcher_.registerMessageCallback<Tom::TestEcho>(
		std::bind(&Network::OnTestEcho, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

int32_t Network::StopServer()
{
    printf("StopServer\n");
    tom::net::CloseNetService(server);
    return 0;
} 
