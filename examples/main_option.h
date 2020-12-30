#pragma once
#include <getopt.h>
#include <stdlib.h>
#include <string>
#ifndef WIN32
#include <sys/time.h>
#include <sys/resource.h>
#endif

#define ERROR_SUCCESS 0
#define DefaultTaskCount 100
#define DefaultTaskProcessTimes 1
#define DefaultThreadNum 1
namespace nett 
{
    struct nett_conf
    {
        std::string mod = "";
        std::string ip = "127.0.0.1";
        int iothread = 1;
        int tasks = 1;
        int times = 0;
        int port = 8888;
        int echo = 0;
        int dropclient = 0;
    };
}

inline uint64_t timenow()
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

