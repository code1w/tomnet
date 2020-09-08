#ifndef BASE_PB_CODEC_H
#define BASE_PB_CODEC_H

#include "net/tomnet.h"

#include "base/buffer.h"
#include "base/google-inl.h"

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

#include <string>
#include <memory>
#include <unordered_map>
#include <functional>

#ifdef WIN32
#include <WinSock2.h>  // for ntohl()
#else
#include <arpa/inet.h>  // for ntohl()
#endif                  // _WIN32


namespace tom {
    namespace pb
    {
        typedef std::unordered_map<int32_t , std::string> PbMsgList;
        typedef std::function<std::string (int32_t)> GetPbMsgTypeCb;
        typedef std::shared_ptr<tom::net::MessageHeader> HeaderPtr;
        
        typedef std::shared_ptr<google::protobuf::Message> MessagePtr;


        static int msgsize(const google::protobuf::Message& message)
        {
#if GOOGLE_PROTOBUF_VERSION > 3009002
            int body_size = google::protobuf::internal::ToIntSize(message.ByteSizeLong());
#else
            int body_size = message.ByteSize();
#endif
            return body_size;
        }


        static int encode_default_header(const google::protobuf::Message& message, tom::Buffer* buf, int body_size) 
        {
            tom::net::MessageHeader header;
            const std::string& typeName = message.GetTypeName();
            int32_t nameLen = static_cast<int32_t>(typeName.size());

            int32_t packetLen = sizeof(header.u.type_name.type_) + sizeof(header.u.type_name.namelen_) + nameLen + body_size;
            buf->ensureWritableBytes(packetLen);
            int8_t  packetType = EVENT_NETMSG;
            assert(packetLen <= MAX_PACKET_SIZE);

            buf->appendInt32(packetLen); // 总长度中不包括长度本身的字节
            buf->appendInt32(packetType);
            buf->appendInt32(nameLen);
            buf->append(typeName.c_str(), typeName.size());
            return packetLen;
        }

        static int32_t encode_header(const google::protobuf::Message& message, tom::Buffer* buf, int body_size, tom::net::MsgHeaderProtocal headprotol = tom::net::nametype) 
        {
            int32_t size = 0;
            tom::net::MessageHeader header;
            if(headprotol == tom::net::nametype)
            {
                const std::string& typeName = message.GetTypeName();
                int32_t nameLen = static_cast<int32_t>(typeName.size());

                int32_t packetLen = sizeof(header.u.type_name.type_) + sizeof(header.u.type_name.namelen_) + nameLen + body_size;
                buf->ensureWritableBytes(packetLen);
                int8_t  packetType = EVENT_NETMSG;
                assert(packetLen <= MAX_PACKET_SIZE);

                buf->appendInt32(packetLen); // 总长度中不包括长度本身的字节
                buf->appendInt32(packetType);
                buf->appendInt32(nameLen);
                buf->append(typeName.c_str(), typeName.size());
                size = packetLen;
            }

            return size;
        }

        static int encode_body(const google::protobuf::Message& message, tom::Buffer* buf)
        {
            #if GOOGLE_PROTOBUF_VERSION > 3009002
                int body_size = google::protobuf::internal::ToIntSize(message.ByteSizeLong());
            #else
                int body_size = message.ByteSize();
            #endif

            buf->ensureWritableBytes(body_size);
            uint8_t* start = reinterpret_cast<uint8_t*>(buf->beginWrite());
            uint8_t* end = message.SerializeWithCachedSizesToArray(start);
            if (end - start != body_size)
            {
            #if GOOGLE_PROTOBUF_VERSION > 3009002
                 ByteSizeConsistencyError(body_size, google::protobuf::internal::ToIntSize(message.ByteSizeLong()), static_cast<int>(end - start));
            #else
                 ByteSizeConsistencyError(body_size, message.ByteSize(), static_cast<int>(end - start));
            #endif
            }
            buf->hasWritten(body_size);
            return body_size;
        }

        static int encode(const google::protobuf::Message& message, tom::Buffer* buf, tom::net::MsgHeaderProtocal headprotol = tom::net::nametype)
        {
            int body_size = msgsize(message);

            int totalsize = encode_header(message, buf, body_size, headprotol);
            int bsize = encode_body(message, buf);
            assert(bsize == body_size);
            return totalsize;
        }


        static google::protobuf::Message* createPbMessage(const std::string& msgname)
        {
            if(msgname == "")
            {
                return nullptr;
            }

            auto descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(msgname);
            if (!descriptor)
            {
                return nullptr;
            }

            auto prototype =  google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
            if (!prototype)
            {
                return nullptr;
            }
            auto  message = prototype->New();
            return message;
        }

        static google::protobuf::Message* createPbMessageById(int32_t msgid, PbMsgList& list)
        {

            google::protobuf::Message* pbmsg = nullptr;
            auto it = list.find(msgid);
            if(it != list.end())
            {
                pbmsg = createPbMessage(it->second);
            }
            return pbmsg;
        }


       static int32_t asInt32(const char* buf)
        {
            int32_t be32 = 0;
            ::memcpy(&be32, buf, sizeof(be32));
            return ntohl(be32);
        }


        static void decode_header(const std::shared_ptr<tom::Buffer>& buf, tom::net::MessageHeader* header, tom::net::MsgHeaderProtocal headprotol = tom::net::nametype)
        {
            switch(headprotol)
            {
                case tom::net::nametype:
                {
                    header->size_ = buf->readInt32();
                    header->u.type_name.type_ = buf->readInt32();
                    header->u.type_name.namelen_ = buf->readInt32();
                    std::string name = buf->retrieveAsString(header->u.type_name.namelen_);
                    strcpy(header->u.type_name.name,name.c_str());
                }
                break;
                default:
                {

                }
                break;
            }
        }

        static bool  Unzip(const char* data, int32_t len)
        {
            return true;
        }



       static std::shared_ptr<google::protobuf::Message> decode(const std::shared_ptr<tom::Buffer>& buf, const HeaderPtr& header, tom::net::MsgHeaderProtocal headprotol = tom::net::nametype,const GetPbMsgTypeCb&& gettype = nullptr)
       {
           decode_header(buf, header.get(), headprotol);
           MessagePtr message = nullptr;

           if(headprotol == tom::net::nametype)
           {
               auto rawmsg = createPbMessage(header->u.type_name.name);
               if(!rawmsg)
               {
                   return nullptr;
               }

               message.reset(rawmsg);
           }

           if (message && !message->ParseFromArray(buf->peek(), buf->readableBytes()))
           {
               message.reset();
               std::cout << "ParseError" << std::endl;
           }
           return message;
       }

        static std::shared_ptr<google::protobuf::Message> decode(const char* buf, int len)
        {
            int32_t offset = 0;
            int32_t packetLen = asInt32(buf + offset);
            offset += sizeof(int32_t);
            int8_t msg_type = int8_t(*(buf + offset));
            offset+=sizeof(int8_t);
            MessagePtr message;
            int32_t nameLen = asInt32(buf + offset);
            offset+=sizeof(int32_t);

            if (nameLen >= 2)
            {
                std::string typeName(buf + offset, buf + offset + nameLen);
                offset+=nameLen;
                message.reset(createPbMessage(typeName));
                if (message)
                {
                    const char* data = buf + offset;
                    int32_t dataLen = len - offset;
                    if (message->ParseFromArray(data, dataLen))
                    {
#ifdef GAMESH_NET_DEBUG
                        std::cout << "NoError" << std::endl;
#endif
                    }
                    else
                    {

                        std::cout << "ParseError" << std::endl;
                    }
                }
                else
                {
                    std::cout << "UnknownMessageType " << std::endl;

                }
            }
            else
            {
                std::cout << "InvalidNameLen " << std::endl;
            }
            return message;
        }


        static void encode_rpc_call(const std::string& rpcName, int32_t type,  const google::protobuf::Message& message, tom::Buffer* buf)
        {
            assert(rpcName.size() != 0);
            assert(buf->readableBytes() == 0);

            int32_t rpcnameLen = static_cast<int32_t>(rpcName.size());

            const std::string& pbmsgName = message.GetTypeName();
            int32_t nameLen = static_cast<int32_t>(pbmsgName.size());

            #if GOOGLE_PROTOBUF_VERSION > 3009002
                int body_size = google::protobuf::internal::ToIntSize(message.ByteSizeLong());
            #else
                int body_size = message.ByteSize();
            #endif

            int32_t packetLen = sizeof(tom::net::MessageHeader) + sizeof(int32_t) + rpcnameLen + sizeof(int32_t) + sizeof(int32_t) + nameLen + body_size;
            assert(packetLen <= MAX_PACKET_SIZE);

            buf->ensureWritableBytes(packetLen);
            buf->appendInt32(packetLen);    // Head len
            buf->appendInt8(EVENT_CALLRPC); // msg type

            buf->appendInt32(rpcnameLen); // rpc name
            buf->append(rpcName.c_str(), rpcName.size());
            buf->appendInt32(type);      //  rpc type

            buf->appendInt32(nameLen);
            buf->append(pbmsgName.c_str(), pbmsgName.size());

            uint8_t* start = reinterpret_cast<uint8_t*>(buf->beginWrite());
            uint8_t* end = message.SerializeWithCachedSizesToArray(start);
            if (end - start != body_size)
            {
            #if GOOGLE_PROTOBUF_VERSION > 3009002
                 ByteSizeConsistencyError(body_size, google::protobuf::internal::ToIntSize(message.ByteSizeLong()), static_cast<int>(end - start));
            #else
                 ByteSizeConsistencyError(body_size, message.ByteSize(), static_cast<int>(end - start));
            #endif
            }
            buf->hasWritten(body_size);
        }
       
        static std::shared_ptr<google::protobuf::Message> decode_rpc_call(const char* buf, int len, std::string& rpcname, int32_t& type)
        {
            int32_t offset = 0;
            int32_t packetLen = asInt32(buf + offset);
            offset += sizeof(int32_t);
            int8_t msg_type = int8_t(*(buf + offset));
            offset += sizeof(int8_t);
            int32_t rpcnameLen = asInt32(buf + offset);
            offset += sizeof(int32_t);
            std::string rpcName(buf + offset, buf + offset + rpcnameLen);
            offset += rpcnameLen;
            rpcname = rpcName;
            int32_t rpcType = asInt32(buf + offset);
            type = rpcType;
            offset += sizeof(int32_t);
            MessagePtr message;
            int32_t nameLen = asInt32(buf + offset);
            offset += sizeof(int32_t);
            if (nameLen >= 2)
            {
                std::string typeName(buf + offset, buf + offset + nameLen);
                offset += nameLen;
               message.reset(createPbMessage(typeName));
                if (message)
                {
                    const char* data = buf + offset;
                    int32_t dataLen = len - offset;
                    if (message->ParseFromArray(data, dataLen))
                    {
#ifdef GAMESH_NET_DEBUG
                        std::cout << "NoError" << std::endl;
#endif
                    }
                    else
                    {

                        std::cout << "ParseError" << std::endl;
                    }
                }
                else
                {
                    std::cout << "UnknownMessageType " << std::endl;

                }
            }
            else
            {
                std::cout << "InvalidNameLen " << std::endl;
            }
            return message;
        }


    }
}
#endif  // UTIL_PB_CODEC_H
