#ifndef  NET_HANDLER_MANAGER_H
#define  NET_HANDLER_MANAGER_H

#include "tomnet.h"
#include "base/noncopyable.h"
#include "base/tomnet_malloc.h"
#include "base/base_common.h"

#include <unordered_map>
#include <queue>
#include <mutex>
#include <atomic>
#include <stdio.h>

#define MAX_HANDLE_COUNT 50000
namespace tom
{
	namespace net
	{
		class AsiokHandler;
		class HandlerManager : public noncopyable
		{
		public:
			HandlerManager() {};
			~HandlerManager() {}

			static HandlerManager& instance()
			{
				std::call_once(HandlerManager::hm_once_flag_, [&]() {
					if (!value_)
					{
						value_ = new HandlerManager;
					}
				});

				assert(value_ != nullptr);
				return *value_;
			}
		public:
			static std::once_flag hm_once_flag_;
			static HandlerManager* value_;

		private:
			std::unordered_map<uint64_t, AsiokHandler*> handlers_;
			std::mutex mutex_;
			uint32_t maxcount_ = MAX_HANDLE_COUNT;
			std::atomic<uint64_t> handlerid_{1};
			std::vector<AsiokHandler*> asio_hands_;
			
		public:

			uint64_t AllocateHandlerId();
			void Initialize(uint32_t maxcount = MAX_HANDLE_COUNT);
			std::size_t HandlerCount();
		};
	}
}
#endif //  TOM_NET_ASIO_HANDLER_MANAGER_H