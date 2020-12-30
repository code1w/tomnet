#ifndef  PROTOBUFNETWORKING_H
#define  PROTOBUFNETWORKING_H

#include "inetworking.h"
#include <google/protobuf/message.h>
#include "base/tomnet_malloc.h"
namespace tom
{
namespace net
{
class ProtobufNetWork : public INetWorking<google::protobuf::Message>
{

};
}}
#endif 
