#include "net_msg_test_help.h"

#include "common/pb_message_helper.h"
#include "common/codec/pb_dispatcher.h"

#include <functional>
#ifdef WIN32
#include <winsock2.h>  // for ntohl()
#pragma   comment   (lib,"Ws2_32.lib")
#else
#include <arpa/inet.h>  // for ntohl()
#endif    // _WIN32

#include <vector>

extern gamesh::pb::ProtobufDispatcher gDispatcher_;
extern size_t total_count;

extern std::vector<uint32_t> handles_;




void OnAccept(uint32_t handle,void* ud, const gamesh::BufferPtr& msg) {
	//std::cout << "accept new connect  " << handle << std::endl;
	if(total_count)
	{

		//SendInfoList(handle);
		total_count--;
	}
	Server* s = (Server*)ud;
	handles_.push_back(handle);

}


void OnConnected(uint32_t handle, void* ud, const gamesh::BufferPtr& msg) {
	//std::cout << " connected remote server handle : " << handle << std::endl;
	//gamesh::net::CloseLink(handle);
	Client* c = (Client*)ud;
//	SendInfoList(handle);
	SendDaobatuRegServer(handle);
	//handles_.push_back(handle);
}

void OnReConnected(uint32_t handle,void* ud, const gamesh::BufferPtr& msg) {
	//std::cout << " connected remote server handle : " << handle << std::endl;
	//gamesh::net::CloseLink(handle);
	SendInfoList(handle);
	//handles_.push_back(handle);
}



void OnConnectFail(uint32_t handle, void* ud, const gamesh::BufferPtr& msg) {
	//std::cout << " connected remote server handle : " << handle << std::endl;
	//gamesh::net::CloseLink(handle);

}
void OnClose(uint32_t handle,void* ud, const gamesh::BufferPtr& msg)
{
	//gamesh::net::CloseLink(handle);
	//std::cout << "net close handle " << handle << std::endl;
}

void OnError(uint32_t handle, const gamesh::BufferPtr& msg) {

}


void OnPersonaInfoList(uint32_t handle, void* ud,const gamesh::HeaderPtr& header, const std::shared_ptr<tellist::personal_info_list>& message)
{
	//std::cout << "Recv pb msg handler : " << handle << ", msgname: " << message->GetTypeName() << std::endl;
#if 0
	int info_size = message->info_size();
	for (int idx = 0; idx < info_size; idx++)
	{
		auto info = message->mutable_info(idx);
		uint32_t id = info->id();
		std::string name = info->name();
		uint32_t age = info->age();
		uint64_t gender = info->gender();
	}
#endif
	//SendInfoList(handle);

}

void OnBufCommonMessage(uint32_t nethandle, void* ud, const gamesh::HeaderPtr& header, const std::shared_ptr<bus::CommonMessage>& message)
{
	auto code = message->code();
	//std::cout << "Recv pb msg handler : " << handle << ", msgname: " << message->GetTypeName() << std::endl;
#if 0
	int info_size = message->info_size();
	for (int idx = 0; idx < info_size; idx++)
	{
		auto info = message->mutable_info(idx);
		uint32_t id = info->id();
		std::string name = info->name();
		uint32_t age = info->age();
		uint64_t gender = info->gender();
	}
#endif
	//SendInfoList(handle);

}


void RegisterCb()
{
	gDispatcher_.registerMessageCallback<tellist::personal_info_list>(
		std::bind(OnPersonaInfoList, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,std::placeholders::_4));

	gDispatcher_.registerMessageCallback<bus::CommonMessage>(
		std::bind(OnBufCommonMessage, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,std::placeholders::_4));
}


void SendInfoList(uint32_t handle)
{
	tellist::personal_info* info;
	tellist::personal_info_list info_list;

	// 构造1K的包
	int32_t idx = 0;
	uint32_t id = 0;
	char name[32];
	uint32_t age;
	uint32_t gender;
	uint64_t phone;
	for (idx = 0; idx < 44 * total_count; idx++) {
		id = idx;
		sprintf(name, "zhang %u", id);
		age = 13 + idx;
		gender = id % 3;
		phone = static_cast<uint64_t>(1234567890 + id);

		info = info_list.add_info();
		info->set_id(id);
		info->set_name(name);
		info->set_age(age);
		info->set_gender((tellist::gender_type)(gender));
		info->set_phone_num(phone);
	}
    gamesh::SendMsg(handle, info_list);
}

void SendSSCommonMsg(uint32_t handle)
{
	ss_proto::CommonMessage msg;
	ss_proto::ServerReport* report = msg.mutable_serverreport();
    report->set_ip("127.0.0.1");
	report->set_count(1000000);
	report->set_port(8888);
	msg.set_code(ss_proto::NONE);
	gamesh::SendMsg(handle, msg);
}

void SendDaobatuRegServer(uint32_t handle)
{
	bus::CommonMessage msg;
	msg.set_code(bus::REG_SVR);
	bus::RegSvrInfo* regs = msg.mutable_svrinfo();
	bus::NewSvrInfo* src =  regs->mutable_src();
	src->set_id(1);
	src->set_ip("127.0.0.1");
	src->set_port("1234");

	bus::NewSvrInfo* dest =  regs->mutable_dest();
	dest->set_id(2);
	dest->set_ip("127.0.0.1");
	dest->set_port("5678");

	auto buffer = std::make_shared<gamesh::Buffer>();
	int totalsize = gamesh::pb::encode(msg, buffer.get(), gamesh::net::seqidzip);
	//gamesh::SendMsg(handle, msg);
	gamesh::SendMsg(handle, msg, 1, 0, 0);

}


void TestConcurrentqueue()
{
	{
		auto packet = std::make_shared<gamesh::Buffer>();
		moodycamel::ConcurrentQueue<std::shared_ptr<gamesh::Buffer>> wbufferlist_;
		wbufferlist_.enqueue(packet);
	}

}

void TestEcodeAndDecode()
{
	tellist::personal_info* info;
	tellist::personal_info_list info_list;

	// 构造1K的包
	int32_t idx = 0;
	uint32_t id = 0;
	char name[32];
	uint32_t age;
	uint32_t gender;
	uint64_t phone;
	for (idx = 0; idx < 44  ; idx++) {
		id = idx;
		sprintf(name, "zhang %u", id);
		age = 13 + idx;
		gender = id % 3;
		phone = static_cast<uint64_t>(1234567890 + id);

		info = info_list.add_info();
		info->set_id(id);
		info->set_name(name);
		info->set_age(age);
		info->set_gender((tellist::gender_type)(gender));
		info->set_phone_num(phone);
	}

	
	auto buffer = std::make_shared<gamesh::Buffer>();
	int totalsize = gamesh::pb::encode(info_list, buffer.get());
	auto header = std::make_shared<gamesh::net::MessageHeader>();
	gamesh::pb::decode(buffer,header);

}

void TestEcodeAndDecodeDaobatuProto()
{
	ss_proto::CommonMessage msg;
	ss_proto::ServerReport* report = msg.mutable_serverreport();
	report->set_ip("127.0.0.1");
	report->set_count(1000000);
	report->set_port(8888);
	msg.set_code(ss_proto::NONE);

	auto buffer = std::make_shared<gamesh::Buffer>();
	int totalsize = gamesh::pb::encode(msg, buffer.get(), gamesh::net::seqidzip);
	auto header = std::make_shared<gamesh::net::MessageHeader>();
	gamesh::pb::decode(buffer,header, gamesh::net::seqidzip);

}



void TestEcodeAndDecodeDaobatuProtoRegister()
{
	bus::CommonMessage msg;
	msg.set_code(bus::REG_SVR);
	bus::RegSvrInfo* regs = msg.mutable_svrinfo();
	bus::NewSvrInfo* src =  regs->mutable_src();
	src->set_id(1);
	src->set_ip("127.0.0.1");
	src->set_port("1234");

	bus::NewSvrInfo* dest =  regs->mutable_dest();
	dest->set_id(2);
	dest->set_ip("127.0.0.1");
	dest->set_port("5678");

	auto buffer = std::make_shared<gamesh::Buffer>();
	int totalsize = gamesh::pb::encode(msg, buffer.get(), gamesh::net::seqidzip);
	auto header = std::make_shared<gamesh::net::MessageHeader>();
	gamesh::pb::decode(buffer, header,gamesh::net::seqidzip);
	//gamesh::SendMsg(handle, msg);

}