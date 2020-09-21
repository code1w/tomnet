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
#include "net/tomnet.h"
#include "base/buffer.h"
#include "base/pb_message_helper.h"
#include "net_msg_test_help.h"

namespace net_test {

extern tom::net::IMessageQueue* Q_;
extern size_t total_count;
}
using namespace net_test;

void client(char const* host, char const* port, int thread_count, size_t totalcount, size_t session_count)
{
	if (thread_count == 0)
	{
		thread_count = 1;
	}

	tom::net::CreateNetwork(tom::net::asio);
	tom::net::InitNetwork(thread_count);
	std::cout << "Client Start tid:" << std::this_thread::get_id() << std::endl;
	total_count =totalcount;
	for (std::size_t i = 0; i < session_count; i++)
	{
		Client * c = new Client;
		c->handler = i;
		tom::net::Connect(host, atoi(port), &Q_, 10000, 25000, tom::net::nametype,c);
	}
}