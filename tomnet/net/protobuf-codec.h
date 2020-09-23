#pragma once
#include "tomnet.h"

namespace tom
{
	namespace net {
		class ProtobufCodec : IMsgCodec
		{
		public:
			std::shared_ptr<tom::Buffer> GenerateBinaryMessage(IMsgHeader* , Any& any);
			std::shared_ptr<tom::Any> GenerateMessage(IMsgHeader*,std::shared_ptr<tom::Buffer>& buffer);
		};
	}
}