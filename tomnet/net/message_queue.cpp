#include "message_queue.h"
#include "base/buffer.h"
#include <algorithm>
#include <memory>

namespace tom {
	namespace net
	{
		MessageQueue::MessageQueue()
		{
          q_ = new moodycamel::ConcurrentQueue<std::shared_ptr<tom::Buffer>>();
		}

		MessageQueue::~MessageQueue() 
		{ 
			delete q_;
		}

		std::shared_ptr<tom::Buffer> MessageQueue::PopMessage()
		{
			std::shared_ptr<tom::Buffer> packet;
            q_->try_dequeue(packet);
			qsize_.fetch_sub(1);
            return packet;
		}
		

		void MessageQueue::PushMessage(const std::shared_ptr<tom::Buffer>& packet, bool front)
		{
			if (!q_->enqueue(packet))
			{
				printf("PushMessage Error \n");
			}
			qsize_.fetch_add(1);
		}
	}
}