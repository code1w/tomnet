#pragma once
#include "tomnet.h"
#include "base/buffer.h"
#include <cstring>

namespace tom {
namespace net {


class DefaultPacketHeader : public IMsgHeader
{
public:

	int32_t msgtypelen = 0;
	std::string msgtype ;
public:

	DefaultPacketHeader();
	DefaultPacketHeader(bool , bool);
	~DefaultPacketHeader();
public:
	void ToBinary(tom::Buffer& buffer);
	void FromBinary(tom::Buffer& buffer);
};
}}