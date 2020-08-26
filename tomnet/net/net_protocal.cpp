
#include "tomnet.h"


#ifdef _WIN32
#include <winsock2.h>  // for ntohl()
#else
#include <arpa/inet.h>  // for ntohl()
#endif  // _WIN32

namespace tom
{
	namespace net
	{

		uint32_t HeaderSize()
		{
			return sizeof(MessageHeader);
		}

		uint32_t PacketLength(const char* header)
		{
			return ntohl(*((uint32_t*)header));
		}

		uint32_t PacketFilter(const char* header)
		{
			switch ((uint8_t)header[4])
			{
			case EVENT_PING:
				return EVENT_PING;
				break;
			case EVENT_PONG:
				return EVENT_PONG;
				break;
			default:
				return EVENT_NETMSG;
				break;

			}
		}

		uint32_t PacketSystem(uint8_t event, char* header)
		{
			header[4] = event;
			return (uint32_t)sizeof(tom::net::MessageHeader);
		}

		uint32_t PacketFinial(char* header, uint32_t length)
		{
			*(uint32_t*)header = length;
			return length;
		}

	}
}