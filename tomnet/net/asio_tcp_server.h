#ifndef  NET_ASIO_TCP_SERVER_H
#define  NET_ASIO_TCP_SERVER_H
#include "tomnet.h"
#include "base/noncopyable.h"
#include "asio_channel.h"
#include "net_define.h"
#include "event_loop_thread.h"
#include "base/tomnet_malloc.h"


#include "asio/asio.hpp"
#include <memory>

namespace tom
{
	namespace net
	{
		class AsioEventLoop;
		
		class AsioTcpServer : public std::enable_shared_from_this<AsioTcpServer>, ServerStatus, noncopyable 
		{
		private:
			uint32_t max_connect_;
			std::shared_ptr<asio::ip::tcp::acceptor> acceptorptr_; 
			AsioEventLoop* loop_;
			EventLoopThread accept_thread_loop_;
			bool reuse_addr_ = false;
			MsgHeaderProtocal headprotol_ = nametype;
			NetMessageCallback messagecb_ = nullptr;
			void * ud_;
			std::string ip_;
			uint16_t port_;
			std::string srvkey_;
			std::vector<uint64_t> myhandlers_; // 连接到这个端口的连接

		public:
			IMessageQueue* msgqueue_ = nullptr;

		public:
			AsioTcpServer(AsioEventLoop* loop , IMessageQueue* queue, uint32_t maxconn);
			AsioTcpServer(AsioEventLoop* loop , uint32_t maxconn);

			~AsioTcpServer()
			{
			};

			bool Start(const char* address, uint16_t port);
			void Stop();
			void Accept();
			void AsyncAcceptCallback(const std::shared_ptr<AsioServerHandler>& handler);
			AsioEventLoop* GetAcceptLoop();
			void SetReuseAddr(bool value){reuse_addr_ = value; }
			void SetMsgHeaderProtocal(MsgHeaderProtocal type) { headprotol_ = type;}
			MsgHeaderProtocal GetMsgHeaderProtocal(){return headprotol_;}
    		void SetUserData(void* ud) { ud_ = ud; }
			void* GetUserdata() const { return ud_; }
			void SetMessageCb(const NetMessageCallback& cb){messagecb_ = cb;}

		};


}
}

#endif   //  TOM_NET_ASIO_TCP_SERVER_H
