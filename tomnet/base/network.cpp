#include "network.h"
#include "net/tomnet.h"
#include "pb_codec.h"
//#include "loging.h"
namespace tom {


namespace net {
Network::Network()
{
}

Network::~Network()
{

}

bool Network::Init(int netthread)
{
    tom::net::CreateNetwork(tom::net::asio);
    tom::net::InitNetwork(netthread);

    return true;
}

bool Network::TcpServer(const std::string& ip, int port)
{
    auto ret = tom::net::StartNetService(ip.c_str(), port, &net_event_queue_, 10000, 250000, 100, tom::net::nametype, nullptr);
    if (ret == 0)
    {
        //LOG_TRACE << "Network::TcpServer 失败! " << ip.c_str() << ":" << port;
        return false;
    }
    return true;
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

    while (net_event_queue_->Size() > 0)
    {
        auto event = net_event_queue_->PopMessage();
        if (!event)
            break;
        ProcessNetEvent(event);
    }

}

void Network::ProcessNetEvent(const std::shared_ptr<tom::Buffer>& event)
{
    tom::net::NetContext* ctx = (tom::net::NetContext*)event->peek();
    event->retrieve(sizeof(tom::net::NetContext));

    // GALOG_DEBUG("%s ud=%p event_type=%d", __FUNCTION__, ctx->ud_, ctx->evetype_);

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
            printf("%s unknown event %d\n", __FUNCTION__, ctx->evetype_);
        }
        break;
    }  // switch
}

void Network::OnAccept(tom::net::NetContext* ctx)
{
    const uint32_t net_handler_id = ctx->handler_;
    printf("%s handler=%lu\n", __FUNCTION__, net_handler_id);

    //auto player = std::make_unique<Player>(net_handler_id);  // new player

    tom::net::LinkReady(net_handler_id, nullptr);
}

void Network::OnClose(tom::net::NetContext* ctx)
{
    const uint32_t net_handler_id = ctx->handler_;
    printf("%s handler=%lu\n", __FUNCTION__, net_handler_id);

    tom::net::SetUserData(net_handler_id, nullptr);
}


void Network::OnConnected(tom::net::NetContext* ctx)
{
    const uint32_t net_handler_id = ctx->handler_;
    printf("%s handler=%lu\n", __FUNCTION__, net_handler_id);

    tom::net::SetUserData(net_handler_id, nullptr);
}

void Network::OnReConnected(tom::net::NetContext* ctx)
{
    const uint32_t net_handler_id = ctx->handler_;
    printf("%s handler=%lu\n", __FUNCTION__, net_handler_id);

    tom::net::SetUserData(net_handler_id, nullptr);
}


bool Network::SendMessage(uint32_t net_handler_id, const google::protobuf::Message& msg)
{
    tom::Buffer buf;
    tom::pb::encode(msg, &buf);

    const uint16_t size = buf.readableBytes();
    tom::net::SendPacket(net_handler_id, buf.peek(), size);
    return true;
}


}}