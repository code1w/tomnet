#include <getopt.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <signal.h>

#include "net/tomnet.h"
#include "network.h"
#include "main_option.h"

#ifndef WIN32
#include <sys/time.h>
#include <sys/resource.h>
#endif


#define ERROR_SUCCESS 0
#define ERROR_ERROR -1
#define DefaultTaskCount 100
#define DefaultTaskProcessTimes 1
#define DefaultThreadNum 1

namespace nett 
{


int tasks = 1;
int times = 0;
int iothread = 1;
std::string ip = "127.0.0.1";
int port = 8888;
std::string mod = "";
int echo = 0;
bool show_help = false;
int dropclient = 0;

#define SharedOptions() \
    {"mod", required_argument, 0, 'm'}, \
    {"tasks", optional_argument, 0, 'c'}, \
    {"times", optional_argument, 0, 'n'}, \
    {"iothread", optional_argument, 0, 't'}, \
    {"ip", optional_argument, 0, 'h'}, \
    {"port", optional_argument, 0, 'p'}, \
    {"echo", optional_argument, 0, 'e'}, \
    {"dropclient", optional_argument, 0, 'd'}, \


#define ProcessSharedOptions() \
    case 'm': \
        mod = optarg; \
    break; \
    case 'c': \
        tasks = atoi(optarg); \
    break; \
    case 'n': \
        times = atoi(optarg); \
    break; \
    case 't': \
        iothread = atoi(optarg); \
    break; \
    case 'h': \
        ip = optarg; \
    break; \
    case 'p': \
        port = atoi(optarg); \
    break; \
    case 'e': \
        echo = atoi(optarg); \
    break; \
    case 'd': \
        dropclient = atoi(optarg); \
    break; \


void showhelp()
{
    printf("Usage: \n");
    printf("-m [required run module s (is server) or c (is client) \n");
    printf("-t [optional io Thread Number default 1\n");
    printf("-h [optional ip default 127.0.0.1 \n");
    printf("-p [optional port default 8888 \n");
    printf("-e [optional test echo default 0 \n");
    printf("-c [optional connect to server nums default 1 \n");
    printf("-d [optional when server accept new connect close it, test client reconnect default 0 \n");

}

int discovery_options(int argc, char** argv)
{
    int ret = ERROR_SUCCESS;

    static option long_options[] = {
        SharedOptions()
        {"vod", no_argument, 0, 'o'},
        {0, 0, 0, 0}
    };

    int opt = 0;
    int option_index = 0;
    while((opt = getopt_long(argc, argv, "m:h:p:c:n:t:e:d:", long_options, &option_index)) != -1){
        switch(opt){
            ProcessSharedOptions()
            default:
                show_help = true;
                break;
        
        }
        ret = ERROR_ERROR;
    }
    return ret;
}

uint64_t tcp_server = 0;
bool g_stop = false;
uint64_t timebase = 0; 
uint64_t frame_delay = 2000;
uint64_t frameslap = 0;
uint64_t current()
{
    return timenow() - timebase;
}
Network* net = nullptr;
bool start = false;
void Update()
{

    timebase =  timenow();
    frameslap = current() + frame_delay;
    
    while (!g_stop)
    {
        while(current() >= frameslap)
        {
            frameslap += frame_delay;
            if(start)
            {
                //net->StopServer();
                //start = false;
            }
            //printf("tick \n");

        }
        net->Update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void SignalHandler(int signum)
{

    if (SIGINT == signum)
    {
        printf("Received signal [%s]\n", SIGINT == signum ? "SIGINT" : "Other");
        tom::net::CloseNetService(tcp_server);
        g_stop = true;
    }
    if (SIGQUIT == signum)
    {
        printf("Received signal [%s]\n", SIGQUIT == signum ? "SIGQUIT" : "Other");
    }
    
}
} // namespace nett

void testbuffer(tom::Buffer& buf)
{
    for(int x = 0; x < 16 ; x++)
    {
        for(int i =0 ;  i < 1024; i++)
        {
            char data[1024] = {0};
            buf.append(data,1024);
        }
    }
}

int main(int argc, char** argv)
{
    tom::Buffer buf;
    testbuffer(buf);
    nett::nett_conf* conf = new nett::nett_conf;
    if(argc < 2)
    {
        nett::showhelp();
        delete conf;
        return 0;
    }

    int ret = nett::discovery_options(argc, argv);
    if(nett::mod == "")
    {
        nett::showhelp();
        return 0;
    }

    tom::net::MessageHeader * header = new tom::net::MessageHeader;
    printf("sizeof MessageHeader type_name %d, %d\n" , sizeof(header->size_) , sizeof(header->u.type_name));
    printf("sizeof void* size  %d\n" , sizeof(void*));


    conf->mod = nett::mod;
    conf->iothread = nett::iothread;
    conf->echo = nett::echo;
    conf->tasks = nett::tasks;
    conf->ip = nett::ip;
    conf->port = nett::port;
    conf->dropclient = nett::dropclient;

    printf("conf : mod %s , io %d, echo %d, tasks %d, ip %s , port %d, dropclient %d \n", conf->mod.c_str(), 
        conf->iothread, conf->echo, conf->tasks, conf->ip.c_str(), conf->port, conf->dropclient);

    nett::net = new Network();
    nett::net->Init(conf);
    if(nett::echo > 0)
    {
        nett::net->isecho = true;
    }

    if(nett::mod =="s")
    {
        //signal(SIGINT,nett::SignalHandler);
        //~signal(SIGQUIT,nett::SignalHandler);
        nett::tcp_server = nett::net->TcpServer(nett::ip, nett::port);
        nett::start = true;
    }
    else if(nett::mod =="c")
    {
        for(int i =1 ; i <= nett::tasks; i++)
        {
            nett::net->TcpClient(nett::ip, nett::port, NULL);
        }
    }


   
    
    nett::Update();
    int i;
    std::cin>> i;
    delete conf;
    delete nett::net;
    return 0;
}