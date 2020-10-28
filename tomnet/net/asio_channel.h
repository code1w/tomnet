#ifndef  NET_ASIO_CHANNEL_H
#define  NET_ASIO_CHANNEL_H

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
#include <concurrentqueue/concurrentqueue.h>
#endif // __GNUC__


#include <memory>
#include <mutex>
#include <atomic>
#include <queue>
#include <array>

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
			tom::Buffer bodybuffer_;
			tom::Buffer headbuffer_;
			tom::Buffer recvbuf_;
			std::array<char, 4096> inputbuf_;
			uint32_t handler_ = 0;
			std::thread::id tid_;
			ConnectedCallback connectedcb_;
			ConnectedCallback reconnectedcb_;
			NetBufferCallback messagecb_;
			NetErrorCallback errorcb_;
			NetCloseCallback disconnectcb_;
			int32_t nsize_ = 0;
			std::string remoteip_;
			uint16_t remoteport_;
			asio::steady_timer reconnectimer_;

			moodycamel::ConcurrentQueue<std::shared_ptr<tom::Buffer>> wbufferlist_;
			std::atomic<bool> sendding_{false};
			std::atomic<bool> start_{ false };
		private:
			uint32_t PostPacket(const std::shared_ptr<tom::Buffer>& packet);
			void AsyncWriteSomeCallback(const std::error_code& error, const std::shared_ptr<tom::Buffer>& packet, std::size_t bytes_transferred);
			void AsyncSendData(const std::shared_ptr<tom::Buffer>& packet);
			void AsyncReadError(const std::error_code& error);
			void ResetBuffer();
		public:
			AsioChannel(AsioEventLoop* loop, std::size_t block_size);
			~AsioChannel();

			asio::ip::tcp::socket& Socket();
			void Connect(const std::string& ip, uint16_t port, bool tryconnect = false);
			void Start();
			int32_t SendPacket(const char* data, uint16_t len);
			bool IsConnect() {return handler_ == 0;	}
			void SetHandler(uint32_t handler) { handler_ = handler; }
			void Close(uint32_t handle);
			void SetSocketOpt();
			void DelayReConnect(const std::string& ip, uint16_t port);
			void DoReConnect(const std::string& ip, uint16_t port);
			void CloseSocket();
			void AsyncRead();
			void TryCeneratePacket();

	
		public:
			void SetConnnectedCb(const ConnectedCallback& cb) { connectedcb_ = cb; }
			void SetReConnnectedCb(const ConnectedCallback& cb) { reconnectedcb_ = cb; }
			void SetMessageCb(const NetBufferCallback& cb) { messagecb_ = cb; }
			void SetErrorCb(const NetErrorCallback& cb) { errorcb_ = cb; }
			void SetCloseCb(const NetCloseCallback& cb) { disconnectcb_ = cb; }
		};

		

	}
}

#endif //  TOM_NET_ASIO_CHANNEL_H
