#include "network.h"
#include "net/net_base.h"
#include "net/pb_codec.h"
#include "loging.h"

Network::Network()
{
}

Network::~Network()
{

}

bool Network::Init(int netthread)
{
    gamesh::net::CreateNetwork(gamesh::net::asio);
    gamesh::net::InitNetwork(netthread);

    return true;
}

bool Network::TcpServer(const std::string& ip, int port)
{
    auto ret = gamesh::net::StartNetService(ip.c_str(), port, &net_event_queue_, 10000, 250000, 100, gamesh::net::nametype, nullptr);
    if (ret == 0)
    {
        LOG_TRACE << "Network::TcpServer 失败! " << ip.c_str() << ":" << port;
        return false;
    }
    return true;
}

bool Network::TcpClient(const std::string& ip, int port, void* ud)
{
    auto ret =  gamesh::net::Connect(ip.c_str(), port, &net_event_queue_, 10000, 25000, gamesh::net::nametype,ud);
    if(ret < 0)
    {
        LOG_TRACE << "Network::TcpClient 失败! " << ip.c_str() << ":" << port;
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

void Network::ProcessNetEvent(const std::shared_ptr<gamesh::Buffer>& event)
{
    gamesh::net::NetContext* ctx = (gamesh::net::NetContext*)event->peek();
    event->retrieve(sizeof(gamesh::net::NetContext));

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
            static auto header = std::make_shared<gamesh::net::MessageHeader>();
            auto msg = gamesh::pb::decode(event, header, gamesh::net::nametype);
            if (msg)
            {
                dispatcher_.onMessage(ctx->handler_, ctx->ud_, header, msg);
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

void Network::OnAccept(gamesh::net::NetContext* ctx)
{
    const uint32_t net_handler_id = ctx->handler_;
    printf("%s handler=%lu\n", __FUNCTION__, net_handler_id);

    //auto player = std::make_unique<Player>(net_handler_id);  // new player

    gamesh::net::LinkReady(net_handler_id, nullptr);
}

void Network::OnClose(gamesh::net::NetContext* ctx)
{
    const uint32_t net_handler_id = ctx->handler_;
    printf("%s handler=%lu\n", __FUNCTION__, net_handler_id);

    gamesh::net::SetUserData(net_handler_id, nullptr);
}


void Network::OnConnected(gamesh::net::NetContext* ctx)
{
    const uint32_t net_handler_id = ctx->handler_;
    printf("%s handler=%lu\n", __FUNCTION__, net_handler_id);

    gamesh::net::SetUserData(net_handler_id, nullptr);
}

void Network::OnReConnected(gamesh::net::NetContext* ctx)
{
    const uint32_t net_handler_id = ctx->handler_;
    printf("%s handler=%lu\n", __FUNCTION__, net_handler_id);

    gamesh::net::SetUserData(net_handler_id, nullptr);
}


bool Network::SendMessage(uint32_t net_handler_id, const google::protobuf::Message& msg)
{
    gamesh::Buffer buf;
    gamesh::pb::encode(msg, &buf, gamesh::net::seqidzip);

    const uint16_t size = buf.readableBytes();
    gamesh::net::SendPacket(net_handler_id, buf.peek(), size);
    return true;
}


