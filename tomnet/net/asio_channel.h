#ifndef NET_ASIO_CHANNEL_H
#define NET_ASIO_CHANNEL_H

#include "net_define.h"
#include "base/noncopyable.h"
#include "base/buffer.h"
#include "asio/asio.hpp"

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include <concurrentqueue/concurrentqueue.h>
#pragma GCC diagnostic pop
#else
#include "concurrentqueue/concurrentqueue.h"
#endif // __GNUC__


#include <memory>
#include <mutex>
#include <atomic>
#include <queue>
#include <functional>
#include <array>
#include "asio_handler.h"

namespace tom
{
	namespace net
	{

		class AsioEventLoop;
		class NetworkPacket;
		class AsioChannel : public std::enable_shared_from_this<AsioChannel>, noncopyable
		{
		private:
			AsioEventLoop* loop_ = nullptr;
			asio::ip::tcp::socket socket_;
			std::size_t  block_size_;
			uint32_t failed_count_ = 0; 
			MessageHeader headerbuffer_{0,0};
			tom::Buffer recvbuf_;
			std::array<char, 4096> inputbuf_;
			tom::Buffer writebuf_;
			uint32_t handler_ = 0;
			std::thread::id tid_;
			ConnectedCallback connectedcb_;
			ConnectedCallback reconnectedcb_;
			NetMessageCallback messagecb_;
			NetErrorCallback errorcb_;
			NetCloseCallback disconnectcb_;
			NetWriteErrorCallback socketwriteerrcb_;
			std::weak_ptr<AsiokHandler> handleweakptr_;
			int32_t nsize_ = 0;
			std::string remoteip_;
			uint16_t remoteport_;
			asio::steady_timer reconnectimer_;

			std::atomic<bool> start_{ false };
			std::atomic<bool> closeing_{ false };
		private:
			uint32_t PostPacket(const std::shared_ptr<tom::Buffer>& packet);
			void AsyncWriteSomeCallback(const std::error_code& error,  std::size_t bytes_transferred);
			void AsyncSendData();
			void AsyncReadError(const std::error_code& error);
		public:
		 	AsioChannel(AsioEventLoop* loop, std::size_t block_size);
			~AsioChannel();

			asio::ip::tcp::socket& Socket();
			void Connect(const std::string& ip, uint16_t port, bool tryconnect = false);
			void Start();
			int32_t SendPacket(const char* data, uint32_t len);
			int32_t SendPacket(const std::shared_ptr<tom::Buffer>& pPacket, uint16_t size);
			bool IsConnect() {return handler_ == 0;	}
			void SetHandler(uint32_t handler) { handler_ = handler; }
			void Close(uint32_t handle);
			void SetSocketOpt();
			void DelayReConnect(const std::string& ip, uint16_t port);
			void DoReConnect(const std::string& ip, uint16_t port);
			void CloseSocket();
			void AsyncRead();
			void TryCeneratePacket();
			void CleanCallback();
	
		public:
			void SetConnnectedCb(const ConnectedCallback& cb) { connectedcb_ = cb; }
			void SetReConnnectedCb(const ConnectedCallback& cb) { reconnectedcb_ = cb; }
			void SetMessageCb(const NetMessageCallback& cb) { messagecb_ = cb; }
			void SetErrorCb(const NetErrorCallback& cb) { errorcb_ = cb; }
			void SetCloseCb(const NetCloseCallback& cb) { disconnectcb_ = cb; }
			void RegisterHandler(const std::shared_ptr<AsiokHandler>& handleptr){handleweakptr_ = handleptr;}
			void SetWriteErrorCb(const NetWriteErrorCallback& cb) { socketwriteerrcb_ = cb; }
		};

		

	}
}

#endif // NET_ASIO_CHANNEL_H
