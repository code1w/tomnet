#ifndef NET_ASIO_TCPSERVER_MANAGER_H
#define NET_ASIO_TCPSERVER_MANAGER_H

#include "tomnet.h"
#include "base/noncopyable.h"


#include <unordered_map>
#include <queue>
#include <mutex>
#include <atomic>
#include <stdio.h>
#include <memory>

namespace tom
{
	namespace net
	{
		class TcpServerManager : public noncopyable
		{
		public:
			static TcpServerManager& instance()
			{
				std::call_once(TcpServerManager::hm_once_flag_, [&]() {
					if (!value_)
					{
						value_ = new TcpServerManager;
					}
				});

				assert(value_ != nullptr);
				return *value_;
			}
		public:
			static std::once_flag hm_once_flag_;
			static TcpServerManager* value_;

		private:
			std::unordered_map<std::string, std::vector<uint32_t>> tcplinks_; // 连接到某一个端口的连接 k(ip:port) v(handle)
			std::unordered_map<std::string, uint64_t > tcpservers_;            // 启动成功的tcpserver k(AsioTcpServer address) v(ip:port)
		public:
			void  AddSomePortTcpServer(const std::string& srvkey, uint64_t);
			void  DelSomePortTcpServer(const std::string& srvkey);
		};
	}
}
#endif // NET_ASIO_HANDLER_MANAGER_H