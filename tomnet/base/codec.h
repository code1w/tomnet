#ifndef BASE_CODEC_H
#define BASE_CODEC_H
#include "net/tomnet.h"
#include "base/pb_codec.h"
#include "base/buffer.h"
namespace tom {
typedef std::shared_ptr<tom::net::MessageHeader> HeaderPtr;
}
#endif  
