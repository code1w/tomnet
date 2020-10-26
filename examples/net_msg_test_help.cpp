#include "net_msg_test_help.h"

#include "base/pb_message_helper.h"
#include "base/pb_dispatcher.h"

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
}
using namespace net_test;

void OnAccept(uint32_t handle,void* ud, const tom::BufferPtr& msg) {
	//std::cout << "accept new connect  " << handle << std::endl;
	if(total_count)
	{

		//SendInfoList(handle);
		total_count--;
	}
	Server* s = (Server*)ud;
	handles_.push_back(handle);
	//SendReqLogin(handle);

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
	std::cout << " OnConnected remote server handle : " << handle << std::endl;
	SendReqLogin(handle);
}

void OnReConnected(uint32_t handle,void* ud, const tom::BufferPtr& msg) {
	//std::cout << " connected remote server handle : " << handle << std::endl;
	//tom::net::CloseLink(handle);
	//SendInfoList(handle);
	//handles_.push_back(handle);
}



void OnConnectFail(uint32_t handle, void* ud, const tom::BufferPtr& msg) {
	//std::cout << " connected remote server handle : " << handle << std::endl;
	//tom::net::CloseLink(handle);

}
void OnClose(uint32_t handle,void* ud, const tom::BufferPtr& msg)
{
	//tom::net::CloseLink(handle);
	//std::cout << "net close handle " << handle << std::endl;
}

void OnError(uint32_t handle, const tom::BufferPtr& msg) {

}


void OnPersonaInfoList(uint32_t handle, void* ud, const std::shared_ptr<Tom::personal_info_list>& message)
{

}

void OnReqLogin(uint32_t handle, void* ud, const std::shared_ptr<Tom::ReqLogin>& message)
{
	std::cout << message->account() <<" , "<< message->passward()<<std::endl;
/*
	Tom::LoginOk rsp;
	Tom::PlayerBaseInfo* pinfo = rsp.mutable_playerbaseinfo();
	pinfo->set_name("zhang xiao bin");
	pinfo->set_playerid(51886);

	//asio::steady_timer timer(io_service_);
	//timer.expires_after(std::chrono::seconds(10));
	//std::cout << "wait 10 s " << std::endl;
	//timer.wait();

	{

	std::cout << " send msg " << std::endl;
	tom::SendMsg(handle, rsp);
	}
	//SendInfoList(handle);
*/
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
	while (true)
	{

	Tom::ReqLogin req;
	req.set_account("zxb-1");
	req.set_passward("1234546");
	auto tname = typeid(Tom::ReqLogin).name();
	tom::SendMsg(handle, req);
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

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

