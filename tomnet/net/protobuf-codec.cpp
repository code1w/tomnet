#include "base/google-inl.h"
#include "protobuf-codec.h"
#include "default_packet_header.h"

#include <google/protobuf/message.h>
using namespace google::protobuf;
namespace tom
{
	namespace net {
		std::shared_ptr<tom::Buffer> ProtobufCodec::GenerateBinaryMessage(Unknown* header, std::shared_ptr<Message>& message)
		{
			auto defheader = (DefaultPacketHeader*)header;

			std::shared_ptr<tom::Buffer> buf = std::make_shared<tom::Buffer>();

#if GOOGLE_PROTOBUF_VERSION > 3009002
			int body_size = internal::ToIntSize(message->ByteSizeLong());
#else
			int body_size = message.ByteSize();
#endif

			buf->ensureWritableBytes(body_size);
			uint8_t* start = reinterpret_cast<uint8_t*>(buf->beginWrite());
			uint8_t* end = message->SerializeWithCachedSizesToArray(start);
			if (end - start != body_size)
			{
#if GOOGLE_PROTOBUF_VERSION > 3009002
				ByteSizeConsistencyError(body_size, internal::ToIntSize(message->ByteSizeLong()), static_cast<int>(end - start));
#else
				ByteSizeConsistencyError(body_size, message.ByteSize(), static_cast<int>(end - start));
#endif
			}
			buf->hasWritten(body_size);

			return buf;
		}

		std::shared_ptr<Message> ProtobufCodec::GenerateMessage(Unknown* header, std::shared_ptr<tom::Buffer>& buffer)
		{
			auto defheader = (DefaultPacketHeader*)header;
			return nullptr;
		}
	}
}