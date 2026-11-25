#include "argparse/argparse.hpp"
#include <signal.h>
#include <stdlib.h>
#include <string>
#include <thread>

#include "main_option.h"
#include "net/tomnet.h"
#include "network.h"

#ifndef WIN32
#include <sys/resource.h>
#include <sys/time.h>
#endif

#define ERROR_SUCCESS 0
#define ERROR_ERROR -1
#define DefaultTaskCount 100
#define DefaultTaskProcessTimes 1
#define DefaultThreadNum 1

namespace nett {

int tasks = 1;
int times = 0;
int iothread = 1;
std::string ip = "127.0.0.1";
int port = 8888;
std::string mod = "s";
int echo = 0;
bool show_help = false;
int dropclient = 0;

void showhelp() {
  printf("Usage: \n");
  printf("-m [required run module s (is server) or c (is client) \n");
  printf("-t [optional io Thread Number default 1\n");
  printf("-h [optional ip default 127.0.0.1 \n");
  printf("-p [optional port default 8888 \n");
  printf("-e [optional test echo default 0 \n");
  printf("-c [optional connect to server nums default 1 \n");
  printf("-d [optional when server accept new connect close it, test client "
         "reconnect default 0 \n");
}

uint64_t tcp_server = 0;
bool g_stop = false;
uint64_t timebase = 0;
uint64_t frame_delay = 2000;
uint64_t frameslap = 0;
uint64_t current() { return timenow() - timebase; }
Network *net = nullptr;
bool start = false;
void Update() {

  timebase = timenow();
  frameslap = current() + frame_delay;

  while (!g_stop) {
    while (current() >= frameslap) {
      frameslap += frame_delay;
      if (start) {
        // net->StopServer();
        // start = false;
      }
      // printf("tick \n");
    }
    net->Update();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

} // namespace nett

void testbuffer(tom::Buffer &buf) {
  for (int x = 0; x < 16; x++) {
    for (int i = 0; i < 1024; i++) {
      char data[1024] = {0};
      buf.append(data, 1024);
    }
  }
}

int main(int argc, char **argv) {
  tom::Buffer buf;
  testbuffer(buf);
  nett::nett_conf *conf = new nett::nett_conf;

  tom::net::MessageHeader *header = new tom::net::MessageHeader;
  printf("sizeof MessageHeader type_name %d, %d\n", sizeof(header->size_),
         sizeof(header->u.type_name));
  printf("sizeof void* size  %d\n", sizeof(void *));

  conf->mod = nett::mod;
  conf->iothread = nett::iothread;
  conf->echo = nett::echo;
  conf->tasks = nett::tasks;
  conf->ip = nett::ip;
  conf->port = nett::port;
  conf->dropclient = nett::dropclient;

  printf("conf : mod %s , io %d, echo %d, tasks %d, ip %s , port %d, "
         "dropclient %d \n",
         conf->mod.c_str(), conf->iothread, conf->echo, conf->tasks,
         conf->ip.c_str(), conf->port, conf->dropclient);

  nett::net = new Network();
  nett::net->Init(conf);
  if (nett::echo > 0) {
    nett::net->isecho = true;
  }

  if (nett::mod == "s") {
    // signal(SIGINT,nett::SignalHandler);
    //~signal(SIGQUIT,nett::SignalHandler);
    nett::tcp_server = nett::net->TcpServer(nett::ip, nett::port);
    nett::start = true;
  } else if (nett::mod == "c") {
    for (int i = 1; i <= nett::tasks; i++) {
      nett::net->TcpClient(nett::ip, nett::port, NULL);
    }
  }

  nett::Update();
  int i;
  std::cin >> i;
  delete conf;
  delete nett::net;
  return 0;
}