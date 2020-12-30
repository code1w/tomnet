#include "tomnet.h"
#include "asio_network.h"

namespace tom
{
namespace net
{
	int32_t(*InitNetwork)(uint32_t workthreads) = 0;
	uint64_t(*StartNetService)(const char* address, uint16_t port, IMessageQueue** msg_queue_ptr, uint32_t ping_invent, uint32_t timeout, uint16_t acceptor_count, MsgHeaderProtocal, void* ) = 0;
	int32_t(*CloseNetService)(uint64_t server_h) = 0;
	int32_t(*Connect)(const char* ip, uint16_t port, IMessageQueue** msg_queue, uint32_t ping_invent, uint32_t timeout, MsgHeaderProtocal, void*) = 0;
	int32_t(*SendPacket)(uint32_t handle, const char* data, uint32_t size) = 0;
	int32_t(*SendBuffer)(uint32_t handle, const tom::Buffer&,const tom::Buffer&) = 0;
	int32_t(*CloseLink)(uint32_t handle) = 0;
	int32_t(*SetUserData)(uint32_t handle, void* ud) = 0;
	int32_t(*LinkReady)(uint32_t handle, void* ud) = 0;
	MsgHeaderProtocal(*GetLinkMsgHeaderProtocal)(uint32_t handle) = 0;
	void(*FreeNetPackage)(uint32_t handle, const std::shared_ptr<tom::Buffer>& package) = 0;


#define SETUP_NET_DRIVER( PIX, FNAME ) 	FNAME = tom::net::PIX##FNAME


	bool CreateNetwork(NetDriver type)
	{
		if (type == asio)
		{
			SETUP_NET_DRIVER(Asio_, InitNetwork);
			SETUP_NET_DRIVER(Asio_, StartNetService);
			SETUP_NET_DRIVER(Asio_, CloseNetService);
			SETUP_NET_DRIVER(Asio_, Connect);
			SETUP_NET_DRIVER(Asio_, SendPacket);
			SETUP_NET_DRIVER(Asio_, CloseLink);
			SETUP_NET_DRIVER(Asio_, SetUserData);
			SETUP_NET_DRIVER(Asio_, SendBuffer);
			SETUP_NET_DRIVER(Asio_, GetLinkMsgHeaderProtocal);
			SETUP_NET_DRIVER(Asio_, LinkReady);

		}
		return true;
	}

	void DestroyNetwork()
	{
	};



} // namespace net
} // namespace tom
