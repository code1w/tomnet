#ifndef  INETWORKING_H
#define  INETWORKING_H

#include "tomnet.h"

namespace tom
{
namespace net
{
	// T 2进制消息类型
	template<typename T> 
	class INetWorking
	{
	public:
		INetWorkProtocol<T>* protocol_;
		IMessageQueue* msgQ_;  
	public:
		void Initialize(NetDriver nettype, int32_t threadnum) 
		{

		}
		virtual void Update() = 0;
		virtual int32_t ProcessNetPackect() = 0;
		
		// 创建一个连出去的网络连接 
		virtual int32_t CreateClient() = 0;
		virtual int32_t CreateServer() = 0;



	};
}
}
#endif 


