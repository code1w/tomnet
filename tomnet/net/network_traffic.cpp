#include "network_traffic.h"
#include "event_loop_thread_pool.h"

#include <stdio.h>
#include <time.h>
#include <string>
#include <chrono>

namespace tom
{
	namespace net
	{
		std::once_flag NetworkTraffic::hm_once_flag_;
		std::once_flag NetworkTraffic::timer_once_falag_;
		NetworkTraffic* NetworkTraffic::value_ = nullptr;
		IMessageQueue* NetworkTraffic::trafmsgq_ = nullptr;

		NetworkTraffic::NetworkTraffic()
			:timer_(nullptr)
			, io_service_(nullptr)
		{

		}

		NetworkTraffic::~NetworkTraffic()
		{
			delete timer_;
		}

		void NetworkTraffic::Print()
		{
			if (recv_packets_.load() == 0)
			{
				//return;
			}
			double avgrecv_packet_size = static_cast<double>(recv_bytes_.load()) / static_cast<double>(recv_packets_.load());
			double recv_throughput = static_cast<double>(recv_bytes_.load()) / (1024*1024);
			printf("Throughput :  %f MiB/s \n", recv_throughput);
			printf("Average Packet Size : %f B\n", avgrecv_packet_size);
			printf("Recv Packet :  %llu n/s\n", recv_packets_.load());
			printf("Post Upstream MsgNum :  %llu n/s \n", post_msg_.load());
			printf("Current MsgQueue MsgNum : %d \n", trafmsgq_->Size());
			printf("IO Thread Num : %d \n",EventLoopThreadPool::instance().ThreadNum());
			printf("Links Num : %llu\n", links_.load());
			time_t timep;
			time (&timep);
			char tmp[64];
			strftime(tmp, sizeof(tmp), "%H:%M:%S",localtime(&timep) );
			printf("System Time  %s \n", tmp);
			printf("=======================================\n");

		}

		void NetworkTraffic::Report()
		{
			Print();
			Reset();
		}

		void NetworkTraffic::OnTimer(const std::error_code& error)
		{
			Print();
			Reset();
			AddTimer(1000);
		}

		void NetworkTraffic::AddTimer(uint64_t deadline)
		{
			
			timer_->expires_from_now(std::chrono::milliseconds(deadline));
			timer_->async_wait(std::bind(&NetworkTraffic::OnTimer, this, std::placeholders::_1));
		}

		void NetworkTraffic::Reset()
		{
			//links_.store(0);
			recv_packets_.store(0);
			send_packets_.store(0);
			recv_bytes_.store(0);
			send_bytes_.store(0);
			post_msg_.store(0);
		}

	}}