#include "message_queue.h"
#include "base/buffer.h"
#include <algorithm>
#include <memory>

namespace tomnet {
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
            return packet;
		}
		

		void MessageQueue::PushMessage(const std::shared_ptr<tom::Buffer>& packet, bool front)
		{
			if (!q_->enqueue(packet))
			{
				printf("PushMessage Error \n");
			}
		}
	}
}