#include "event_loop_thread_pool.h"
#include "asio_event_loop.h"

#include "net_define.h"
namespace tom
{
	namespace net
	{
        std::once_flag EventLoopThreadPool::new_once_flag_;
        EventLoopThreadPool* EventLoopThreadPool::value_ = nullptr;
        std::once_flag EventLoopThreadPool::start_once_flag_;
		EventLoopThreadPool::EventLoopThreadPool()
		{

		}

		EventLoopThreadPool::~EventLoopThreadPool()
		{
			Join();
			threads_.clear();
		}

        void EventLoopThreadPool::Initialize(uint32_t thread_number)
        {
            thread_num_ = thread_number;
        }


        bool EventLoopThreadPool::Start(bool wait_thread_started) {
            status_.store(kStarting);
            if (thread_num_ == 0) {
                status_.store(kRunning);
                return true;
            }

            std::shared_ptr<std::atomic<uint32_t>> started_count(new std::atomic<uint32_t>(0));
            std::shared_ptr<std::atomic<uint32_t>> exited_count(new std::atomic<uint32_t>(0));
            for (uint32_t i = 0; i < thread_num_; ++i) {
                auto prefn = [this, started_count]() {
                    this->OnThreadStarted(started_count->fetch_add(1) + 1);
                    return EventLoopThread::kOK;
                };

                auto postfn = [this, exited_count]() {
                    this->OnThreadExited(exited_count->fetch_add(1) + 1);
                    return EventLoopThread::kOK;
                };

                EventLoopThreadPtr t(new EventLoopThread());
                if (!t->Start(wait_thread_started, prefn, postfn)) {
                    return false;
                }

                std::stringstream ss;
                ss << "EventLoopThreadPool-thread-" << i << "th";
                t->set_name(ss.str());
                threads_.push_back(t);
            }

            // when all the working thread have started,
            // status_ will be stored with kRunning in method OnThreadStarted

            if (wait_thread_started) {
                while (!IsRunning()) {
                    usleep(1);
                }
                assert(status_.load() == kRunning);
            }

            return true;
        }

        void EventLoopThreadPool::Stop(bool wait_thread_exit) {
            Stop(wait_thread_exit, DoneCallback());
        }

        void EventLoopThreadPool::Stop(DoneCallback fn) {
            Stop(false, fn);
        }

        void EventLoopThreadPool::Stop(bool wait_thread_exit, DoneCallback fn) {
            status_.store(kStopping);

            if (thread_num_ == 0) {
                status_.store(kStopped);

                if (fn) {
                    fn();
                }
                return;
            }

            stopped_cb_ = fn;

            for (auto &t : threads_) {
                t->Stop();
            }

            // when all the working thread have stopped
            // status_ will be stored with kStopped in method OnThreadExited

            auto is_stopped_fn = [this]() {
                for (auto &t : this->threads_) {
                    if (!t->IsStopped()) {
                        return false;
                    }
                }
                return true;
            };

            if (thread_num_ > 0 && wait_thread_exit) {
                while (!is_stopped_fn()) {
                    usleep(1);
                }
            }

            status_.store(kStopped);
        }

        void EventLoopThreadPool::Join() {
            for (auto &t : threads_) {
                t->Join();
            }
            threads_.clear();
        }


        AsioEventLoop* EventLoopThreadPool::GetNextLoop() {
            AsioEventLoop* loop = nullptr;
            if (IsRunning() && !threads_.empty()) {
                int64_t next = next_.fetch_add(1);
                next = next % threads_.size();
                loop = (threads_[next])->loop();
            }
            return loop;
        }

        AsioEventLoop* EventLoopThreadPool::GetNextLoopWithHash(uint64_t hash) {
            AsioEventLoop* loop = nullptr;
            if (IsRunning() && !threads_.empty()) {
                uint64_t next = hash % threads_.size();
                loop = (threads_[next])->loop();
            }
            return loop;
        }

        uint32_t EventLoopThreadPool::ThreadNum() const {
            return thread_num_;
        }

        void EventLoopThreadPool::OnThreadStarted(uint32_t count) {
            if (count == thread_num_) {
                status_.store(kRunning);
            }
        }

        void EventLoopThreadPool::OnThreadExited(uint32_t count) {
            if (count == thread_num_) {
                status_.store(kStopped);
                if (stopped_cb_) {
                    stopped_cb_();
                    stopped_cb_ = DoneCallback();
                }
            }
        }

	}
}
