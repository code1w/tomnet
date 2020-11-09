
#ifndef  NET_ASIO_EVENT_LOOP_H
#define  NET_ASIO_EVENT_LOOP_H

#include "server_status.h"
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include <concurrentqueue/concurrentqueue.h>
#pragma GCC diagnostic pop
#else
#include <concurrentqueue/concurrentqueue.h>
#endif // __GNUC__
#include "base/noncopyable.h"
#include "asio/asio.hpp"
#include "base/tomnet_malloc.h"
#include "asio_handler.h"

#include <functional>
#include <mutex>
#include <atomic>
#include <unordered_map>

namespace tom
{
namespace net
{
	class AsioEventLoop : public ServerStatus
	{
	public:
		typedef std::function<void()> Functor;
	private:
		asio::io_service io_service_;
		std::shared_ptr<asio::io_service::work> io_work_;
		std::thread::id tid_;
		std::mutex mutex_;
		std::atomic<bool> notified_;
		moodycamel::ConcurrentQueue<Functor>* pending_functors_;
		std::atomic<int> pending_functor_count_;
		std::unordered_map<uint64_t, AsiokHandler*> handlers_;

	public:
		AsioEventLoop();
		~AsioEventLoop();

		void Run();
		void Stop();
		void RunInLoop(const Functor& handler);
		void QueueInLoop(const Functor& handler);
		void RunInLoop(Functor&& handler);
		void QueueInLoop(Functor&& handler);

		void RunInIoService(const Functor& handler);
		void RunInIoService(Functor&& handler);

		void AddHandler(uint64_t, AsiokHandler*);
		AsiokHandler* FetchAsioHandler(uint64_t);
		void RemoveHandler(uint64_t);

	private:
		void Init();
		void StopInLoop();
		void DoPendingFunctors();
		std::size_t GetPendingQueueSize();
		bool IsPendingQueueEmpty();
	public:
		// TODO : timer
		//InvokeTimerPtr RunAfter(double delay_ms, const Functor& f);
		//InvokeTimerPtr RunAfter(Duration delay, const Functor& f);
		//InvokeTimerPtr RunEvery(Duration interval, const Functor& f);
		//InvokeTimerPtr RunAfter(double delay_ms, Functor&& f);
		//InvokeTimerPtr RunAfter(Duration delay, Functor&& f);
		//InvokeTimerPtr RunEvery(Duration interval, Functor&& f);

	public:
		asio::io_service& io_service() {
			return io_service_;
		}
		bool IsInLoopThread() const {
			return tid_ == std::this_thread::get_id();
		}
		int pending_functor_count() const {
			return pending_functor_count_.load();
		}
		const std::thread::id& tid() const {
			return tid_;
		}

	};
}
}

#endif