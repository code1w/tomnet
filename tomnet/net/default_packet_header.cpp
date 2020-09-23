#include "default_packet_header.h"

namespace tom {
namespace net {
DefaultPacketHeader::DefaultPacketHeader()
{
	packetlen = 0;
	packetop = 0;
	encrypted = false;
	compressed = false;
	binary = true;
}

DefaultPacketHeader::DefaultPacketHeader(bool encrypted = false,bool compressed = false)
{
	packetlen = 0;
	packetop = 0;
	encrypted = encrypted;
	compressed = compressed;
	binary = true;
}

DefaultPacketHeader::~DefaultPacketHeader()
{

}

void DefaultPacketHeader::ToBinary(tom::Buffer& buffer)
{
	buffer.appendInt32(packetlen);
	buffer.appendInt32(packetop);
}

void DefaultPacketHeader::FromBinary(tom::Buffer& buffer)
{
	packetlen = buffer.readInt32();
	packetop = buffer.readInt32();
	msgtypelen = buffer.readInt32();
	msgtype = buffer.retrieveAsString(msgtypelen);
}

}}