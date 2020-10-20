#ifndef  PROTOBUFNETWORKING_H
#define  PROTOBUFNETWORKING_H

#include "inetworking.h"
#include <google/protobuf/message.h>

namespace tom
{
namespace net
{
class ProtobufNetWork : public INetWorking<google::protobuf::Message>
{

};
}}
#endif 
