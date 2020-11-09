#ifndef  INETWORKING_H
#define  INETWORKING_H

#include "tomnet.h"
#include "base/tomnet_malloc.h"
namespace tom
{
namespace net
{
	// T 2������Ϣ����
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
		
		// ����һ������ȥ���������� 
		virtual int32_t CreateClient() = 0;
		virtual int32_t CreateServer() = 0;



	};
}
}
#endif 


