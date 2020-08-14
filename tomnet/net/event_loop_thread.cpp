#include "asio_event_loop.h"
#include "event_loop_thread.h"
#include " TOM_NET_define.h"

namespace tomnet
{
namespace net
{

    EventLoopThread::EventLoopThread()
        : event_loop_(new AsioEventLoop) {
    }

    EventLoopThread::~EventLoopThread() {
        assert(IsStopped());
        Join();
    }

    bool EventLoopThread::Start(bool wait_thread_started, Functor pre, Functor post) {
        status_ = kStarting;

        assert(thread_.get() == nullptr);
        thread_.reset(new std::thread(std::bind(&EventLoopThread::Run, this, pre, post)));

        if (wait_thread_started) {
            while (status_ < kRunning) {
                usleep(1);
            }
        }
        return true;
    }

    void EventLoopThread::Run(const Functor& pre, const Functor& post) {
        if (name_.empty()) {
            std::ostringstream os;
            os << "thread-" << std::this_thread::get_id();
            name_ = os.str();
        }

        auto fn = [this, pre]() {
            status_ = kRunning;
            if (pre) {
                auto rc = pre();
                if (rc != kOK) {
                    event_loop_->Stop();
                }
            }
        };
        //event_loop_->QueueInLoop(std::move(fn));
        event_loop_->RunInIoService(std::move(fn));
        event_loop_->Run();

        if (post) {
            post();
        }

        assert(event_loop_->IsStopped());
        status_ = kStopped;
    }

    void EventLoopThread::Stop(bool wait_thread_exit) {
        assert(status_ == kRunning && IsRunning());
        status_ = kStopping;
        event_loop_->Stop();

        if (wait_thread_exit) {
            while (!IsStopped()) {
                usleep(1);
            }
            Join();
        }
    }

    void EventLoopThread::Join() {
        std::lock_guard<std::mutex> guard(mutex_);
        if (thread_ && thread_->joinable()) {
            try {
                thread_->join();
            } catch (const std::system_error& e) {
            }
            thread_.reset();
        }
    }

    void EventLoopThread::set_name(const std::string& n) {
        name_ = n;
    }

    const std::string& EventLoopThread::name() const {
        return name_;
    }


    AsioEventLoop* EventLoopThread::loop() {
        return event_loop_.get();
    }


    boost::asio::io_service* EventLoopThread::io_service() {
        return &(loop()->io_service());
    }

    std::thread::id EventLoopThread::tid() const {
        if (thread_) {
            return thread_->get_id();
        }

        return std::thread::id();
    }

    bool EventLoopThread::IsRunning() const {
        return event_loop_->IsRunning();
    }


}
}
