#pragma once
#include "tomnet.h"
#include "base/google-inl.h"
#include "base/misc.h"
#include <google/protobuf/message.h>
#include "protobuf-codec.h"

using namespace google::protobuf;
namespace tom {
	namespace net{
		TOM_TOM_NET_DLL_DECL class DefaultNetWorkProtocol : public INetWorkProtocol<Message>
		{
		public:
			DefaultNetWorkProtocol(IMsgHeader* header, ProtobufCodec* codec)
				:INetWorkProtocol<Message>(header, (IMsgCodec<Message>*)codec)
			{
			}


			std::shared_ptr<tom::Buffer> PackNetPacket(const Message& message)
			{

				auto buffer = std::make_shared<tom::Buffer>();
				DefaultPacketHeader packetheader;

				packetheader.MessageHeaderBuffer(message, buffer); //
				if(codec_)
				{
					codec_->GenerateBinaryMessage(buffer, message);
				}

				if(packetheader.compressed)
				{
                   // TODO compressed
				}
				
				if(packetheader.encrypted)
				{
                   // TODO encrypted
				}
				return buffer;
			}

			std::shared_ptr<Message> UnPackNetPacket(std::shared_ptr<tom::Buffer>& buffer)
			{
				DefaultPacketHeader header;
				
				header.MessageHeaderFromBuffer(buffer);
				if (header.encrypted)
				{

				}

				if (header.compressed)
				{

				}


				auto message = codec_->GenerateMessage(header.msgtype, buffer);
				return message;
			}

			std::shared_ptr<Message> UnPackNetPacket(tom::Buffer& buffer)
			{
				auto msg = codec_->GenerateMessage(std::string(""), buffer);
				return nullptr;
			}

			std::shared_ptr<tom::Buffer> PackNetPacket(std::shared_ptr<Message>& message) 
			{
				auto buf = codec_->GenerateBinaryMessage(message);
				return buf;
			}




		};
	}}
