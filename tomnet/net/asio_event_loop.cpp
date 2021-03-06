#include "tomnet.h"
#include "asio_event_loop.h"
#include "asio_handler.h"
#include <stdio.h>
namespace tom
{
namespace net{
	AsioEventLoop::AsioEventLoop():notified_(false), pending_functor_count_(0)
	{
		Init();
		io_work_.reset(new asio::io_service::work(io_service_));
	}

	AsioEventLoop::~AsioEventLoop()
	{
		delete pending_functors_;
	}

    void AsioEventLoop::Init() 
	{
        pending_functors_ = new moodycamel::ConcurrentQueue<Functor>();
        tid_ = std::this_thread::get_id(); // The default thread id
    }

	void AsioEventLoop::Run()
	{
		status_.store(kStarting);
		tid_ = std::this_thread::get_id();
		status_.store(kRunning);
		io_service_.run();
		status_.store(kStopped);
	}

	void AsioEventLoop::Stop()
	{
		io_work_.reset();
		io_service_.reset();
		io_service_.stop();
		assert(status_.load() == kRunning);
		status_.store(kStopping);
		QueueInLoop(std::bind(&AsioEventLoop::StopInLoop, this));
	}

	void AsioEventLoop::StopInLoop() {
		assert(status_.load() == kStopping);

		auto f = [this]() {
			for (int i = 0;;i++) {
				DoPendingFunctors();
				if (IsPendingQueueEmpty()) {
					break;
				}
			}
		};
		f();
		io_service_.stop();
	}

	void AsioEventLoop::RunInLoop(const Functor& functor) {
		if (IsRunning() && IsInLoopThread()) {
			functor();
		} else {
			QueueInLoop(functor);
		}
	}

	void AsioEventLoop::RunInLoop(Functor&& functor) {
		if (IsRunning() && IsInLoopThread()) {
			functor();
		} else {
			QueueInLoop(std::move(functor));
		}
	}

	void AsioEventLoop::RunInIoService(const Functor& functor) {
		io_service_.post(functor);
	}

	void AsioEventLoop::RunInIoService(Functor&& functor) {
		io_service_.post(std::move(functor));
	}


	void AsioEventLoop::QueueInLoop(const Functor& cb)
	{
		
		while (!pending_functors_->enqueue(cb)){
		}
		++pending_functor_count_;
		if (!notified_.load())
		{
			notified_.store(true);
		}
	}

	void AsioEventLoop::QueueInLoop(Functor&& cb)
	{
		while (!pending_functors_->enqueue(std::move(cb))){
		}
		++pending_functor_count_;
		if (!notified_.load())
		{
			notified_.store(true);
		}
	}

	void AsioEventLoop::DoPendingFunctors()
	{
		notified_.store(false);
		Functor f;
		while (pending_functors_->try_dequeue(f))
		{
			f();
			--pending_functor_count_;
		}
	}

	std::size_t AsioEventLoop::GetPendingQueueSize() {
		return pending_functors_->size_approx();
	}

	bool AsioEventLoop::IsPendingQueueEmpty() {
		return pending_functors_->size_approx() == 0;
	}


	// accept 线程调用
	void AsioEventLoop::AddHandler(uint32_t hid, const std::shared_ptr<AsiokHandler>& handler)
	{
		std::lock_guard<std::mutex> guard(mutex_);
		if(hid > 0)
		{
			auto it = handlers_.find(hid);
			if(it == handlers_.end())
			{
				handlers_.emplace(hid, handler);
			}
		}
	}
	
	
	std::shared_ptr<AsiokHandler> AsioEventLoop::FetchAsioHandler(uint32_t hid)
	{
		std::lock_guard<std::mutex> guard(mutex_);
		if(hid > 0)
		{
			auto it = handlers_.find(hid);
			if(it != handlers_.end())
			{
				return it->second;

			}
		}
		return nullptr;
	}

	void AsioEventLoop::RemoveHandler(uint32_t hid)
	{
		std::lock_guard<std::mutex> guard(mutex_);
		if(hid > 0)
		{
			auto it = handlers_.find(hid);
			if(it != handlers_.end())
			{
				it->second = nullptr;
				handlers_.erase(it);
			}
		}
	}
}}
