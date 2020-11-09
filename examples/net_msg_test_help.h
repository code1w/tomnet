#pragma once
#include <ctime> 
#include <iostream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mutex>
#include <concurrentqueue/concurrentqueue.h>

#include "base/buffer.h"
#include "net/net_define.h"
#include "base/codec.h"
#include "base/tomnet_malloc.h"
#include "tellist.pb.h"
#include "login.pb.h"

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

 void OnAccept(uint32_t handle, void* ud,const tom::BufferPtr& msg);
 void OnConnected(uint32_t handle, void* ud,  const tom::BufferPtr& msg);
 void OnReConnected(uint32_t handle, void* ud, const tom::BufferPtr& msg);
 void OnClose(uint32_t handle, void* ud, const tom::BufferPtr& msg);
 void OnError(uint32_t handle, void* ud, const tom::BufferPtr& msg);
 void OnConnectFail(uint32_t handle, void* ud,const tom::BufferPtr& msg);
 void SendInfoList(uint32_t handle);
 void SendReqLogin(uint32_t handle);

 void RegisterCb();
 void OnPersonaInfoList(uint32_t nethandle, void* ud, const std::shared_ptr<Tom::personal_info_list>& message);
 void OnReqLogin(uint32_t nethandle, void* ud, const std::shared_ptr<Tom::ReqLogin>& message);

 void TestConcurrentqueue();
 void TestEcodeAndDecode();
