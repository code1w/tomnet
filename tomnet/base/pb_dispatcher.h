#ifndef BASE_PB_DISPATCHER_H
#define BASE_PB_DISPATCHER_H

#include <google/protobuf/message.h>

#include <iostream>
#include <unordered_map>
#include <functional>
#include "base/noncopyable.h"
#include "base/codec.h"

namespace tom
{
namespace pb
{
    template<typename To, typename From>
    inline To implicit_cast(From const& f)
    {
        return f;
    }
    template<typename To, typename From>     // use like this: down_cast<T*>(foo);
    inline To down_cast(From* f)                     // so we only accept pointers
    {
        if (false)
        {
            implicit_cast<From*, To>(0);
        }

#if !defined(NDEBUG) && !defined(GOOGLE_PROTOBUF_NO_RTTI)
        assert(f == NULL || dynamic_cast<To>(f) != NULL);  // RTTI: debug mode only!
#endif
        return static_cast<To>(f);
    }

    template<typename T>
    inline T* get_pointer(const std::shared_ptr<T>& ptr)
    {
        return ptr.get();
    }

    template<typename T>
    inline T* get_pointer(const std::unique_ptr<T>& ptr)
    {
        return ptr.get();
    }

    // Adapted from google-protobuf stubs/common.h

    template<typename To, typename From>
    inline ::std::shared_ptr<To> down_pointer_cast(const ::std::shared_ptr<From>& f) {
        if (false)
        {
            implicit_cast<From*, To*>(0);
        }

#ifndef DEBUG
        assert(f == NULL || dynamic_cast<To*>(get_pointer(f)) != NULL);
#endif
        return ::std::static_pointer_cast<To>(f);
    }

    typedef std::shared_ptr<google::protobuf::Message> MessagePtr;

    class Callback: public noncopyable
    {
     public:
      virtual ~Callback() {};
      virtual void onMessage(uint32_t h, void* u, const MessagePtr& message) const = 0;
    };

    template <typename T>
    class CallbackT : public Callback
    {
     public:
      typedef std::function<void (uint32_t h, void* u, const std::shared_ptr<T>& message)> ProtobufMessageCallback;

      CallbackT(const ProtobufMessageCallback& callback)
        : callback_(callback)
      {
      }

      virtual void onMessage(uint32_t h, void* u,const MessagePtr& message) const
      {
        std::shared_ptr<T> concrete = down_pointer_cast<T>(message);
        assert(concrete != NULL);
        callback_(h, u, concrete);
      }

     private:
      ProtobufMessageCallback callback_;
    };

    class ProtobufDispatcher
    {
     public:
         std::function<void(uint32_t h, void* u, const MessagePtr& message)> defaultCallback_;

      ProtobufDispatcher()
        : defaultCallback_(ProtobufDispatcher::discardProtobufMessage)
      {
      }
      ProtobufDispatcher(std::function<void (uint32_t h, void* u, const MessagePtr& message)> default_callback)
	      : defaultCallback_(default_callback)
      {
      }
      virtual ~ProtobufDispatcher()
      {
	      callbacks_.clear();
      }
      static void discardProtobufMessage(uint32_t h, void* u, const MessagePtr& message)
      {
	  
      }

      void onMessage(uint32_t h, void* u, const MessagePtr& message) const
      {
        CallbackMap::const_iterator it = callbacks_.find(message->GetDescriptor());
        if (it != callbacks_.end())
        {
          it->second->onMessage(h, u, message);
        }
        else
        {
          defaultCallback_(h, u, message);
        }
      }

      template<typename T>
      void registerMessageCallback(const typename CallbackT<T>::ProtobufMessageCallback& callback)
      {
		    std::shared_ptr<CallbackT<T>> pd(new CallbackT<T>(callback));
		    callbacks_[T::descriptor()] = pd;
      }

     typedef std::unordered_map<const google::protobuf::Descriptor*, std::shared_ptr<Callback>> CallbackMap;
     CallbackMap callbacks_;

    };

}
}
#endif //UTIL_PB_DISPATCHER_H