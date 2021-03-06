#ifndef BASE_PB_MESSAGE_HELPER_H
#define BASE_PB_MESSAGE_HELPER_H

#include "codec.h"
#include "base/tomnet_malloc.h"

//TODO : 安装解码器
namespace tom {

#if 0
template< class message_type >
int32_t SendMsg(uint32_t handle, const message_type &msg) {
  
  auto hprotocal = tom::net::GetLinkMsgHeaderProtocal(handle);
  tom::Buffer buf;
  tom::pb::encode(msg, &buf, hprotocal);
  int ret = tom::net::SendPacket(handle, buf.peek(), buf.readableBytes()); 
  return ret;
}
#endif 

static int32_t SendMsg(uint32_t handle, const google::protobuf::Message& msg) 
{

	//auto hprotocal = tom::net::GetLinkMsgHeaderProtocal(handle);
	tom::Buffer buf;
	tom::pb::encode(msg, &buf);
	int ret = tom::net::SendPacket(handle, buf.peek(), buf.readableBytes());
	return ret;
}

template< class message_type >
void BroadcastMsg( uint32_t g, const message_type &msg, bool self = true)
{
}

}
#endif

