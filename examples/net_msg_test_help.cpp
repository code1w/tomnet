#include "net_msg_test_help.h"

#include "base/pb_message_helper.h"
#include "base/pb_dispatcher.h"
#include "net/network_traffic.h"

#include <functional>
#ifdef WIN32
#include <winsock2.h>  // for ntohl()
#pragma   comment   (lib,"Ws2_32.lib")
#else
#include <arpa/inet.h>  // for ntohl()
#endif    // _WIN32

#include "asio/asio.hpp"

#include <vector>
#include <chrono>

namespace net_test {


extern tom::pb::ProtobufDispatcher gDispatcher_;
extern size_t total_count;
extern asio::io_service io_service_;
extern std::vector<uint32_t> handles_;

char * teststr = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";

}
using namespace net_test;

void OnAccept(uint32_t handle,void* ud, const tom::BufferPtr& msg) {
	tom::net::NetworkTraffic::instance().FetchAddLinks();
	tom::net::LinkReady(handle, NULL);
}

void DelaySend(const std::error_code& error)
{
	if (!error)
	{
		// Timer expired.
	}
}


void OnConnected(uint32_t handle, void* ud, const tom::BufferPtr& msg) {
	Client* c = (Client*)ud;
	tom::net::NetworkTraffic::instance().FetchAddLinks();
	SendReqLogin(handle);
}

void OnReConnected(uint32_t handle,void* ud, const tom::BufferPtr& msg) {
	//std::cout << " connected remote server handle : " << handle << std::endl;
	//tom::net::CloseLink(handle);
	//SendInfoList(handle);
	//handles_.push_back(handle);
	std::cout << " ReConnect  server handle : " << handle << std::endl;
	//SendReqLogin(handle);
}

void OnConnectFail(uint32_t handle, void* ud, const tom::BufferPtr& msg) {
	//std::cout << " connected remote server handle : " << handle << std::endl;
	//tom::net::CloseLink(handle);

}
void OnClose(uint32_t handle,void* ud, const tom::BufferPtr& msg)
{
	//tom::net::CloseLink(handle);
	//std::cout << "net close handle " << handle << std::endl;
	tom::net::NetworkTraffic::instance().FetchSubLinks();
}

void OnError(uint32_t handle, const tom::BufferPtr& msg) {

}


void OnPersonaInfoList(uint32_t handle, void* ud, const std::shared_ptr<Tom::personal_info_list>& message)
{

}

void OnReqLogin(uint32_t handle, void* ud, const std::shared_ptr<Tom::ReqLogin>& message)
{
	//std::cout << "OnReqLogin ," << handle << ", "<<message->account() << ", " << message->passward() << std::endl;
	SendReqLogin(handle);
	tom::net::NetworkTraffic::instance().FetchAddRecvByte(message->ByteSizeLong());
}


void RegisterCb()
{
	gDispatcher_.registerMessageCallback<Tom::personal_info_list>(
		std::bind(OnPersonaInfoList, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));


	gDispatcher_.registerMessageCallback<Tom::ReqLogin>(
		std::bind(OnReqLogin, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void SendReqLogin(uint32_t handle)
{
	static int32_t index = 1;
    char pasw[128];
	sprintf(pasw, "passward.%u", handle*100);
	Tom::ReqLogin req;
	req.set_account(teststr);
	req.set_passward(pasw);
	auto tname = typeid(Tom::ReqLogin).name();
	tom::SendMsg(handle, req);
	index++;
	tom::net::NetworkTraffic::instance().FetchAddSendByte(req.ByteSizeLong());
	tom::net::NetworkTraffic::instance().FetchAddSendPacket();
}

void SendInfoList(uint32_t handle)
{
	Tom::personal_info* info;
	Tom::personal_info_list info_list;

	// 构造1K的包
	int32_t idx = 0;
	uint32_t id = 0;
	char name[32];
	uint32_t age;
	uint32_t gender;
	uint64_t phone;
	for (idx = 0; idx < 44 * total_count; idx++) {
		id = idx;
		sprintf(name, "tom %u", id);
		age = 13 + idx;
		gender = id % 3;
		phone = static_cast<uint64_t>(1234567890 + id);

		info = info_list.add_info();
		info->set_id(id);
		info->set_name(name);
		info->set_age(age);
		info->set_gender((Tom::gender_type)(gender));
		info->set_phone_num(phone);
	}
    tom::SendMsg(handle, info_list);
}


void TestConcurrentqueue()
{
	{
		auto packet = std::make_shared<tom::Buffer>();
		moodycamel::ConcurrentQueue<std::shared_ptr<tom::Buffer>> wbufferlist_;
		wbufferlist_.enqueue(packet);
	}

}

void TestEcodeAndDecode()
{
	Tom::personal_info* info;
	Tom::personal_info_list info_list;

	// 构造1K的包
	int32_t idx = 0;
	uint32_t id = 0;
	char name[32];
	uint32_t age;
	uint32_t gender;
	uint64_t phone;
	for (idx = 0; idx < 44  ; idx++) {
		id = idx;
		sprintf(name, "tom %u", id);
		age = 13 + idx;
		gender = id % 3;
		phone = static_cast<uint64_t>(1234567890 + id);

		info = info_list.add_info();
		info->set_id(id);
		info->set_name(name);
		info->set_age(age);
		info->set_gender((Tom::gender_type)(gender));
		info->set_phone_num(phone);
	}

	
	auto buffer = std::make_shared<tom::Buffer>();
	int totalsize = tom::pb::encode(info_list, buffer.get());
	auto header = std::make_shared<tom::net::MessageHeader>();
	tom::pb::decode(buffer,header);

}

