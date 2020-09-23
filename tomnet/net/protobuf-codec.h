#pragma once
#include "tomnet.h"
#include <google/protobuf/message.h>
namespace tom
{
	namespace net {



		using Message = google::protobuf::Message;
		class ProtobufCodec : IMsgCodec<Message>
		{
		public:
			std::shared_ptr<tom::Buffer> GenerateBinaryMessage(Unknown* , std::shared_ptr<Message>&);
			std::shared_ptr<Message> GenerateMessage(Unknown*, std::shared_ptr<tom::Buffer>&);
		};
	}
}