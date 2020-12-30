#pragma once
#include "base/google-inl.h"
#include <google/protobuf/message.h>

using namespace google::protobuf;
namespace tom {
	namespace base
	{
		static int PbMessageSize(const Message& message)
		{
#if GOOGLE_PROTOBUF_VERSION > 3009002
			int body_size = internal::ToIntSize(message.ByteSizeLong());
#else
			int body_size = message.ByteSize();
#endif
			return body_size;
		}
	}
}