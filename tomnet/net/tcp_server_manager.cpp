#include "tcp_server_manager.h"
#include "event_loop_thread_pool.h"
#include "asio_handler.h"
#include "asio_event_loop.h"

namespace tom
{
namespace net
{
    std::once_flag TcpServerManager::hm_once_flag_;
    TcpServerManager* TcpServerManager::value_ = nullptr;
    void TcpServerManager::AddSomePortTcpServer(const std::string& srvkey, uint64_t server)
    {
        auto iter = tcpservers_.find(srvkey);
        if(iter != tcpservers_.end())
        {
            printf("GameshNet AddSomePortTcpServer Repeat %s ", srvkey.c_str());
            return;
        }

        if(server == 0)
        {
            printf("GameshNet AddSomePortTcpServer Error Server Address %s ", srvkey.c_str());
            return;
        }

        tcpservers_.emplace(srvkey, server); 
    }

    void TcpServerManager::DelSomePortTcpServer(const std::string& srvkey)
    {
        auto iter = tcpservers_.find(srvkey);
        if(iter == tcpservers_.end())
        {
            printf("GameshNet DelSomePortTcpServer Server Not Find %s ", srvkey.c_str());
            return;
        }

        tcpservers_.erase(iter);
    }




}}