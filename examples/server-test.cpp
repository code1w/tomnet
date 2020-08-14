#include <algorithm>
#include <iostream>
#include <list>
#include <string.h>
#include <mutex>
#include <assert.h>
#include <vector>
#include <memory>
#include <queue>
#include <stack>
#include <thread>
#include "gamesh_net/net_base.h"
#include "common/buffer.h"
#include "common/pb_message_helper.h"
#include "net_msg_test_help.h"

extern gamesh::net::IMessageQueue* Q_;
extern size_t total_count;
void server(char const* host, char const* port, int thread_count, size_t totalcount)
{
	if (thread_count == 0)
	{
		thread_count = 1;
	}
	Server * s = new Server;
	s->handler = 0;
	s->ip = std::string(host);
	s->port = atoi(port);

	gamesh::net::CreateNetwork(gamesh::net::asio);
	gamesh::net::InitNetwork(thread_count);
	total_count = totalcount;
	std::cout << "Server Start tid:" << std::this_thread::get_id() <<", packlen "<<total_count<< std::endl;
	if (!gamesh::net::StartNetService(host, atoi(port), &Q_, 10000, 25000, 100, gamesh::net::nametype, s))
	{
		std::cout<< "start net fail port " << atoi(port)<< std::endl;
	}

}