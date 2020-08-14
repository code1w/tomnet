#ifndef  TOM_NET_MESSAGE_QUEUE
#define  TOM_NET_MESSAGE_QUEUE

#include "tomnet.h"

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include <concurrentqueue/concurrentqueue.h>
#pragma GCC diagnostic pop
#else
#include <concurrentqueue/concurrentqueue.h>
#endif // __GNUC__



#include <mutex>
#include <list>
#include <queue>

namespace tomnet
{
namespace net 
{
	class MessageQueue : public IMessageQueue
	{
	public:
		MessageQueue();
		~MessageQueue();

		bool IsEmpty() { return q_->size_approx() == 0; }
        std::size_t Size() { return q_->size_approx(); }
		std::shared_ptr<tom::Buffer> PopMessage();
		void PushMessage(const std::shared_ptr<tom::Buffer>&, bool front = false);
	private:
         moodycamel::ConcurrentQueue<std::shared_ptr<tom::Buffer>>* q_;
	};
}
}

#endif  //  TOM_NET_MESSAGE_QUEUE
