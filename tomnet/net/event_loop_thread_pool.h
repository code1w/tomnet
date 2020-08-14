// modify from 360 evpp

#ifndef  TOM_NET_EVENT_LOOP_THREAD_POOL_H
#define  TOM_NET_EVENT_LOOP_THREAD_POOL_H

#include <atomic>
#include <functional>
#include <memory>
#include <vector>

#include "event_loop_thread.h"
#include "server_status.h"
#include "base/noncopyable.h"

namespace tomnet
{
namespace net
{
	class AsioEventLoop;
	class EventLoopThreadPool : public ServerStatus, noncopyable
	{
	public:
		typedef std::function<void()> DoneCallback;
		EventLoopThreadPool();
		~EventLoopThreadPool();

		static EventLoopThreadPool& instance()
		{
			std::call_once(EventLoopThreadPool::new_once_flag_, [&]() {
				if (!value_)
				{
					value_ = new EventLoopThreadPool;
				}
				});
			assert(value_ != nullptr);
			return *value_;
		}

		void Initialize(uint32_t thread_num);
		bool Start(bool wait_thread_started = false);
		void Stop(bool wait_thread_exit = false);
		void Stop(DoneCallback fn);

		void Join();

	public:
		static std::once_flag new_once_flag_;
		static EventLoopThreadPool* value_;
		static std::once_flag start_once_flag_;

		AsioEventLoop* GetNextLoop();
		AsioEventLoop* GetNextLoopWithHash(uint64_t hash);
		uint32_t ThreadNum() const;
	private:
		void Stop(bool wait_thread_exit, DoneCallback fn);
		void OnThreadStarted(uint32_t count);
		void OnThreadExited(uint32_t count);

	private:
		uint32_t thread_num_ = 0;
		std::atomic<int64_t> next_ = {0};
		DoneCallback stopped_cb_;

		typedef std::shared_ptr<EventLoopThread> EventLoopThreadPtr;
		std::vector<EventLoopThreadPtr> threads_;

	};
}
}
#endif
