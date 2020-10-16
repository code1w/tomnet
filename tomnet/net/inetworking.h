#ifndef  INETWORKING_H
#define  INETWORKING_H

#include "tomnet.h"

namespace tom
{
namespace net
{
	template<typename T>
	class INetWorking
	{
	public:
		INetWorkProtocol<T>* protocol_;
	public:
		virtual void Update() = 0;
		virtual int ProcessNetPackect(IMessageQueue* msg_queue) = 0;

	};
}
}
#endif 


