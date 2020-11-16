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
#include <unordered_map>

namespace net_test {

struct ClientData
{
uint32_t handle  = 0;
int32_t echotimes = 0;
};

extern tom::pb::ProtobufDispatcher gDispatcher_;
extern size_t total_count;
extern asio::io_service io_service_;
std::unordered_map<uint32_t, ClientData*> handles_;
extern std::string style ;
std::unordered_map<std::string , uint64_t> gtimes_;

char * teststr = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";

}
using namespace net_test;

uint64_t timenow()
{
    uint64_t _t;
#ifdef __linux__
                struct timeval t;
                gettimeofday(&t, nullptr);
                _t = ((uint64_t)t.tv_sec) * 1000 + t.tv_usec/1000;
#elif WIN32
                FILETIME ft;
                SYSTEMTIME st;
                GetSystemTime(&st);
                SystemTimeToFileTime(&st, &ft);
                _t = ((uint64_t)ft.dwHighDateTime) << 32 | ft.dwLowDateTime;
                //const uint64_t offset = 116444736000000000;
                _t -= (uint64)116444736000000000;
                _t /= 10000;
#endif
    return _t;
}

void OnAccept(uint32_t handle,void* ud, const tom::BufferPtr& msg) {
	tom::net::NetworkTraffic::instance().FetchAddLinks();
	ClientData * data = new ClientData;
	data->handle = handle;
	data->echotimes = 0;
	handles_[handle] = data;
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
	SendTestEcho(handle);
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

int64_t sendsize = 0;

void OnTestEcho(uint32_t handle, void* ud, const std::shared_ptr<Tom::TestEcho>& message)
{
	static int64_t spackage = 0;
	if(style == "c")
	{
		static int64_t timebase = timenow();
		if(timenow() - timebase >= 1000)
		{
			int64_t tt = timenow() - message->time();
			std::cout << "Echo RTT " << tt << " ms"<< std::endl;
			std::cout << "Echo Package  " << spackage << " n/s"<< std::endl;
			std::cout << "Echo Package Len " << (sendsize/1024) << "kB/s" << std::endl;
			spackage = 0;
			timebase = timenow(); 
			sendsize = 0;

		}
		SendTestEcho(handle);
		spackage++;
	}
	else if(style == "s")
	{
		static int64_t stimebase = timenow();
		if(timenow() - stimebase >= 1000)
		{
			auto it = handles_.begin();
			for(; it != handles_.end(); ++it)
			{
				if(it->second->echotimes<=0)
				{
					std::cout << "handle timeout : " << it->first << std::endl;
				}
				else{
					std::cout << "echo handle  : " << it->first << ", "<<it->second->echotimes<<  std::endl;
					it->second->echotimes = 0;
				}
			}

			stimebase = timenow(); 

		}
		handles_[handle]->echotimes++;
		SendTestEcho(handle, message);
		//tom::SendMsg(handle, *(message.get()));
		tom::net::NetworkTraffic::instance().FetchAddRecvByte(message->ByteSizeLong());
	}


}


void RegisterCb()
{
	gDispatcher_.registerMessageCallback<Tom::personal_info_list>(
		std::bind(OnPersonaInfoList, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));


	gDispatcher_.registerMessageCallback<Tom::ReqLogin>(
		std::bind(OnReqLogin, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	gDispatcher_.registerMessageCallback<Tom::TestEcho>(
		std::bind(OnTestEcho, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
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

void SendTestEcho(uint32_t handle, const std::shared_ptr<Tom::TestEcho>& message)
{
	static int32_t x = 1;
    char index[128];
	sprintf(index, "%u", x);
	Tom::TestEcho req;
	req.set_index(index);
	req.set_msg(teststr);
	if(message)
	{
		req.set_time(message->time());
	}
	else{

		// 客户端连接后发本地时间
		int64_t time = timenow();
		req.set_time(time);
	}

	tom::SendMsg(handle, req);

	if(style == "c")
	{
		sendsize+=req.ByteSizeLong();
	}
	
	x++;
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

