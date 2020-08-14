#ifndef  TOM_NET_ASIO_TCP_SERVER_H
#define  TOM_NET_ASIO_TCP_SERVER_H
#include "tomnet.h"
#include "base/noncopyable.h"
#include "asio_channel.h"
#include " TOM_NET_define.h"
#include "event_loop_thread.h"



#include "boost/asio.hpp"
#include <memory>

namespace tomnet
{
	namespace net
	{
		class AsioEventLoop;
		
		class AsioTcpServer : public std::enable_shared_from_this<AsioTcpServer>, noncopyable
		{
		private:
			uint32_t max_connect_;
			std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptorptr_; 
			AsioEventLoop* loop_;
			EventLoopThread accept_thread_loop_;
			bool reuse_addr_ = false;
			MsgHeaderProtocal headprotol_ = nametype;
			void * ud_;
		public:
			IMessageQueue* msgqueue_ = nullptr;

		public:
			AsioTcpServer(AsioEventLoop* loop , IMessageQueue* queue, uint32_t maxconn);

			~AsioTcpServer()
			{
				accept_thread_loop_.Stop(true);
			};

			bool Start(const char* address, uint16_t port);
			void Stop();
			void Accept();
			void AsyncAcceptCallback(AsioServerHandler* handler, const boost::system::error_code& ec);
			void AsyncAcceptCallback(const boost::system::error_code& ec);
			AsioEventLoop* GetAcceptLoop();
			void SetReuseAddr(bool value){reuse_addr_ = value; }
			void SetMsgHeaderProtocal(MsgHeaderProtocal type) { headprotol_ = type;}
			MsgHeaderProtocal GetMsgHeaderProtocal(){return headprotol_;}
    		void SetUserData(void* ud) { ud_ = ud; }
			void* GetUserdata() const { return ud_; }

		};

}
}

#endif   //  TOM_NET_ASIO_TCP_SERVER_H
