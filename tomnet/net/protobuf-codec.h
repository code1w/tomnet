#pragma once
#include "tomnet.h"
#include "default-packet-header.h"
#include "base/google-inl.h"

#include <google/protobuf/message.h>
#include <google/protobuf/port_def.inc>
using namespace google::protobuf;
#if GOOGLE_PROTOBUF_VERSION < 3009002
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif 
namespace tom
{
	namespace net {
		using Message = google::protobuf::Message;

		class ProtobufCodec : public IMsgCodec<Message>
		{
		public:
			std::shared_ptr<tom::Buffer> GenerateBinaryMessage(std::shared_ptr<Message>& message)
			{
				std::shared_ptr<tom::Buffer> buf = std::make_shared<tom::Buffer>();

				int body_size = internal::ToIntSize(message->ByteSizeLong());
				buf->ensureWritableBytes(body_size);
				uint8_t* start = reinterpret_cast<uint8_t*>(buf->beginWrite());
				uint8_t* end = message->SerializeWithCachedSizesToArray(start);
				if (end - start != body_size)
				{
					ByteSizeConsistencyError(body_size, internal::ToIntSize(message->ByteSizeLong()), static_cast<int>(end - start));
				}
				buf->hasWritten(body_size);

				return buf;
			}

			std::shared_ptr<Message> GenerateMessage(std::string& msgtype , const std::shared_ptr<tom::Buffer>& buffer)
			{
				if (msgtype == "")
				{
					return nullptr;
				}

				auto descriptor = DescriptorPool::generated_pool()->FindMessageTypeByName(msgtype);
				if (!descriptor)
				{
					return nullptr;
				}

				auto prototype = MessageFactory::generated_factory()->GetPrototype(descriptor);
				if (!prototype)
				{
					return nullptr;
				}

				std::shared_ptr<Message> message(prototype->New());


				if (message && !message->ParseFromArray(buffer->peek(), buffer->readableBytes()))
				{
					message.reset();
					std::cout << "ParseError" << std::endl;
				}
				return message;
			}

			std::shared_ptr<tom::Buffer> GenerateBinaryMessage(const Message& message)
			{
				std::shared_ptr<tom::Buffer> buf = std::make_shared<tom::Buffer>();
				int body_size = internal::ToIntSize(message.ByteSizeLong());
				buf->ensureWritableBytes(body_size);
				uint8_t* start = reinterpret_cast<uint8_t*>(buf->beginWrite());
				uint8_t* end = message.SerializeWithCachedSizesToArray(start);
				if (end - start != body_size)
				{
					ByteSizeConsistencyError(body_size, internal::ToIntSize(message.ByteSizeLong()), static_cast<int>(end - start));
				}
				buf->hasWritten(body_size);

				return buf;
			}

			void GenerateBinaryMessage(tom::Buffer& buffer, const Message& message)
			{

				int body_size = internal::ToIntSize(message.ByteSizeLong());
				buffer.ensureWritableBytes(body_size);
				uint8_t* start = reinterpret_cast<uint8_t*>(buffer.beginWrite());
				uint8_t* end = message.SerializeWithCachedSizesToArray(start);
				if (end - start != body_size)
				{
					ByteSizeConsistencyError(body_size, internal::ToIntSize(message.ByteSizeLong()), static_cast<int>(end - start));
				}
				buffer.hasWritten(body_size);
			}


			std::shared_ptr<Message> GenerateMessage(std::string& msgtype, const tom::Buffer& buffer)
			{
				
				if (msgtype == "")
				{
					return nullptr;
				}

				auto descriptor = DescriptorPool::generated_pool()->FindMessageTypeByName(msgtype);
				if (!descriptor)
				{
					return nullptr;
				}

				auto prototype = MessageFactory::generated_factory()->GetPrototype(descriptor);
				if (!prototype)
				{
					return nullptr;
				}

				std::shared_ptr<Message> message(prototype->New());


				if (message && !message->ParseFromArray(buffer.peek(), buffer.readableBytes()))
				{
					message.reset();
					std::cout << "ParseError" << std::endl;
				}
				return message;
			}

		};
	}
}