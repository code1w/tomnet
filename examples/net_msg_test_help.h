#pragma once
#include <ctime> 
#include <iostream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mutex>
#include "common/buffer.h"
#include "tellist.pb.h"
#include "ss_proto.pb.h"
#include "bus.pb.h"
#include "common/codec.h"

#include <concurrentqueue/concurrentqueue.h>

struct Client
{
	uint32_t handler = 0;
};

struct Server
{
	uint32_t handler = 0;
	std::string ip;
	int16_t port;
};



 void client(char const* host, char const* port, int thread_count, size_t total_count, size_t session_count);
 void server(char const* host, char const* port, int thread_count, size_t total_count);

 void OnAccept(uint32_t handle, void* ud,const gamesh::BufferPtr& msg);
 void OnConnected(uint32_t handle, void* ud,  const gamesh::BufferPtr& msg);
 void OnReConnected(uint32_t handle, void* ud, const gamesh::BufferPtr& msg);
 void OnClose(uint32_t handle, void* ud, const gamesh::BufferPtr& msg);
 void OnError(uint32_t handle, void* ud, const gamesh::BufferPtr& msg);
 void OnConnectFail(uint32_t handle, void* ud,const gamesh::BufferPtr& msg);
 void SendInfoList(uint32_t handle);
 void SendSSCommonMsg(uint32_t handle);

 void RegisterCb();
 void OnPersonaInfoList(uint32_t nethandle, void* ud, const gamesh::HeaderPtr& header, const std::shared_ptr<tellist::personal_info_list>& message);
 void OnBufCommonMessage(uint32_t nethandle, void* ud,const gamesh::HeaderPtr& header, const std::shared_ptr<bus::CommonMessage>& message);

 void TestConcurrentqueue();
 void TestEcodeAndDecode();
 void TestEcodeAndDecodeDaobatuProto();
 void TestEcodeAndDecodeDaobatuProtoRegister();
 void SendDaobatuRegServer(uint32_t handle);