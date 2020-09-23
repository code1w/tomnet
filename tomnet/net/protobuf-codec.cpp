#include "base/google-inl.h"
#include "protobuf-codec.h"
#include "default_packet_header.h"

#include <google/protobuf/message.h>
namespace tom
{
	namespace net {
		typedef std::shared_ptr<google::protobuf::Message> PbPtr;
		typedef std::shared_ptr<tom::Buffer> BufPtr;
		std::shared_ptr<tom::Buffer> ProtobufCodec::GenerateBinaryMessage(IMsgHeader* header,Any& any)
		{
			auto defheader = (DefaultPacketHeader*)header;

			BufPtr buf = std::make_shared<tom::Buffer>();
			PbPtr message = any.Get<PbPtr>();

#if GOOGLE_PROTOBUF_VERSION > 3009002
			int body_size = google::protobuf::internal::ToIntSize(message->ByteSizeLong());
#else
			int body_size = message.ByteSize();
#endif

			buf->ensureWritableBytes(body_size);
			uint8_t* start = reinterpret_cast<uint8_t*>(buf->beginWrite());
			uint8_t* end = message->SerializeWithCachedSizesToArray(start);
			if (end - start != body_size)
			{
#if GOOGLE_PROTOBUF_VERSION > 3009002
				ByteSizeConsistencyError(body_size, google::protobuf::internal::ToIntSize(message->ByteSizeLong()), static_cast<int>(end - start));
#else
				ByteSizeConsistencyError(body_size, message.ByteSize(), static_cast<int>(end - start));
#endif
			}
			buf->hasWritten(body_size);

			return buf;
		}

		std::shared_ptr<tom::Any> ProtobufCodec::GenerateMessage(IMsgHeader* header, std::shared_ptr<tom::Buffer>& buffer)
		{
			auto defheader = (DefaultPacketHeader*)header;
			return nullptr;
		}
	}
}