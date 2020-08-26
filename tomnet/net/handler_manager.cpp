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

		bool HandlerManager::LinkHandler(AsiokHandler* phandler)
		{
			std::lock_guard<std::mutex> l(mutex_);
			handlers_[phandler->GetHandler()] = phandler;
			return true;
		}

		void  HandlerManager::LinkDown(AsiokHandler* handler)
		{
			std::lock_guard<std::mutex> l(mutex_);
			auto it = handlers_.find(handler->GetHandler());
			if (it != handlers_.end())
			{
				delete it->second;
				handlers_.erase(it);
			}
		}

		void  HandlerManager::LinkDown(uint64_t handler)
		{
#ifdef  TOM_ TOM_NET_TEST
			printf("LinkDown handler %d \n", handler);
#endif
#ifdef  TOM_ TOM_NET_TRAFFIC
			NetworkTraffic::instance().FetchSubLinks();
#endif 
			std::lock_guard<std::mutex> l(mutex_);
			auto it = handlers_.find(handler);
			if (it != handlers_.end())
			{
				delete it->second;
				handlers_.erase(it);
			}
		}

		AsiokHandler* HandlerManager::FectHandler(uint64_t handler)
		{
			std::lock_guard<std::mutex> l(mutex_);
			AsiokHandler* phander = nullptr;
			auto kv = handlers_.find(handler);
			if(kv != handlers_.end())
			{
				phander = kv->second;
			}
			return phander;
		}
		


	}
}
