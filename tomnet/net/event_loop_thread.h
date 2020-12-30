// modify from evpp

#ifndef  NET_EVENT_LOOP_THREAD_H
#define  NET_EVENT_LOOP_THREAD_H

#include <thread>
#include <mutex>
#include <iostream>
#include <memory>
#include <functional>

#include "asio/asio.hpp"
#include "server_status.h"
#include "base/tomnet_malloc.h"

namespace tom
{
namespace net
{
	class AsioEventLoop;
	class EventLoopThread : public ServerStatus
	{
	public:
		typedef std::function<int()> Functor;
		enum { kOK = 0 };
		EventLoopThread();
		~EventLoopThread();
		bool Start(bool wait_thread_started = true,
			Functor pre = Functor(),
			Functor post = Functor());

		void Stop(bool wait_thread_exit = false);
		void Join();
	public:
		void set_name(const std::string& n);
		const std::string& name() const;
		AsioEventLoop* loop();
		asio::io_service* io_service();
		std::thread::id tid() const;
		bool IsRunning() const;

	private:
		void Run(const Functor& pre, const Functor& post);
	private:
		std::shared_ptr<AsioEventLoop> event_loop_;
		std::mutex mutex_;
		std::shared_ptr<std::thread> thread_;
		std::string name_;
	};
}
}

#endif 
