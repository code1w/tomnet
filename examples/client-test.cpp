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
void client(char const* host, char const* port, int thread_count, size_t totalcount, size_t session_count)
{
	if (thread_count == 0)
	{
		thread_count = 1;
	}
	gamesh::net::CreateNetwork(gamesh::net::asio);
	gamesh::net::InitNetwork(thread_count);
	std::cout << "Client Start tid:" << std::this_thread::get_id() << std::endl;
	total_count =totalcount;
	for (std::size_t i = 0; i < session_count; i++)
	{
		Client * c = new Client;
		c->handler = i;
        gamesh::net::Connect(host, atoi(port), &Q_, 10000, 25000, gamesh::net::seqidzip,c);
	}
}