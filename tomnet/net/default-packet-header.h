#pragma once
#include "tomnet.h"
#include "base/buffer.h"
#include "base/misc.h"
#include "dll_export.h"
#include <google/protobuf/message.h>
#include <cstring>

using namespace google::protobuf;
namespace tom {
namespace net {

TOM_TOM_NET_DLL_C_DECL class  DefaultPacketHeader : public IMsgHeader
{
public:

	int32_t msgtypelen = 0;
	std::string msgtype ;
public:

	DefaultPacketHeader(bool encrypted = false, bool compressed = false)
	{
		packetlen = 0;
		packetop = 0;
		encrypted = encrypted;
		compressed = compressed;
		binary = true;
	}

	~DefaultPacketHeader()
	{

	}

	void MessageHeaderToBuffer(const Message& message, std::shared_ptr<tom::Buffer>& buffer)
	{
		auto msgsize = tom::base::PbMessageSize(message);
		const std::string& typeName = message.GetTypeName();
		int32_t nameLen = static_cast<int32_t>(typeName.size());
		packetlen = sizeof(packetop) + sizeof(msgtypelen) + nameLen + msgsize;
		buffer->ensureWritableBytes(packetlen);
		buffer->appendInt32(packetlen);  
		buffer->appendInt32(packetop);
		buffer->appendInt32(nameLen);
		buffer->append(typeName.c_str(), typeName.size());


	}

	void MessageHeaderToBuffer(const Message& message, tom::Buffer& buffer)
	{
		auto msgsize = tom::base::PbMessageSize(message);
		const std::string& typeName = message.GetTypeName();
		int32_t nameLen = static_cast<int32_t>(typeName.size());
		packetlen = sizeof(packetop) + sizeof(msgtypelen) + nameLen + msgsize;
		buffer.ensureWritableBytes(packetlen);
		buffer.appendInt32(packetlen);
		buffer.appendInt32(packetop);
		buffer.appendInt32(nameLen);
		buffer.append(typeName.c_str(), typeName.size());


	}

	void MessageHeaderFromBuffer(const std::shared_ptr<tom::Buffer>& buffer)
	{
		packetlen = buffer->readInt32();
		packetop = buffer->readInt32();
		msgtypelen = buffer->readInt32();
		msgtype = buffer->retrieveAsString(msgtypelen);
	}

	void ToBinary(tom::Buffer& buffer)
	{
		buffer.appendInt32(packetlen);
		buffer.appendInt32(packetop);
	}



	void FromBinary(tom::Buffer& buffer)
	{
		packetlen = buffer.readInt32();
		packetop = buffer.readInt32();
		msgtypelen = buffer.readInt32();
		msgtype = buffer.retrieveAsString(msgtypelen);
	}
};
}}