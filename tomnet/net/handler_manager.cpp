#include "handler_manager.h"
#include "network_traffic.h"
#include "asio_handler.h"

namespace tom
{
	namespace net
	{
		std::once_flag HandlerManager::hm_once_flag_;
		HandlerManager* HandlerManager::value_ = nullptr;

		uint64_t HandlerManager::AllocateHandlerId()
		{
			uint64_t handle =  handlerid_.fetch_add(1);
			return handle;
		}

		void HandlerManager::Initialize(uint32_t maxcount)
		{
			maxcount_ = maxcount;
		}

		std::size_t HandlerManager::HandlerCount()
		{
			return handlers_.size();
		}
	}
}
