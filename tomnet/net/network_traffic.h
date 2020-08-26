#ifndef NET_NETWORK_TRAFFIC_H
#define NET_NETWORK_TRAFFIC_H

#include "tomnet.h"
#include "base/noncopyable.h"
#include "boost/asio.hpp"

#include <unordered_map>
#include <queue>
#include <mutex>
#include <atomic>

#define MAX_HANDLE_COUNT 50000
namespace tom
{
	namespace net
	{

		class NetworkTraffic : public noncopyable
		{
		public:
			NetworkTraffic();
			~NetworkTraffic();

			static NetworkTraffic& instance()
			{
				std::call_once(NetworkTraffic::hm_once_flag_, [&]() {
					if (!value_)
					{
						value_ = new NetworkTraffic;
					}
					});

				assert(value_ != nullptr);
				return *value_;
			}
		public:
			static std::once_flag hm_once_flag_;
			static std::once_flag timer_once_falag_;

			static NetworkTraffic* value_;
			static IMessageQueue* trafmsgq_;
		private:
			boost::asio::deadline_timer* timer_;
			boost::asio::io_service* io_service_;
			std::atomic<uint64_t> links_{0}; // ������
			std::atomic<uint64_t> recv_packets_{0}; // ��λʱ�����յ��İ���
			std::atomic<uint64_t> send_packets_{0}; // ��λʱ���ڷ��ĵİ���
			std::atomic<uint64_t> recv_bytes_{0}; // ��λʱ���յ�������
			std::atomic<uint64_t> send_bytes_{0}; // 
			std::atomic<uint64_t> post_msg_{0}; // 
			int32_t threadnum_;

		public:
			void OnTimer(const boost::system::error_code& error);
			void AddTimer(uint64_t);
			void Print();
			void Reset();

			void FetchAddLinks()
			{
				links_.fetch_add(1);
			}

			void FetchSubLinks()
			{
				links_.fetch_sub(1);
			}

			void FetchAddRecvPacket()
			{
				recv_packets_.fetch_add(1);
			}

			void FetchAddSendPacket()
			{
				send_packets_.fetch_add(1);
			}

			void FetchAddRecvByte(uint64_t b)
			{
				recv_bytes_.fetch_add(b);
			}

			void FetchAddSendByte(uint64_t b)
			{
				send_bytes_.fetch_add(b);
			}

			void FetchAddPostMsg()
			{
				post_msg_.fetch_add(1);
			}

			void CreateTimer(boost::asio::io_service* io)
			{
				std::call_once(NetworkTraffic::timer_once_falag_, [&](){
					io_service_ = io;
					timer_ = new boost::asio::deadline_timer(*io_service_);
					Reset();
					auto fn = [&]() {
						AddTimer(1000);
					};
					io_service_->post(fn);
				});

			}
		};
	}
}
#endif 
