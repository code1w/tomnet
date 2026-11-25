#pragma once
// #include <getopt.h>
#include <chrono>
#include <stdlib.h>
#include <string>
#ifndef WIN32
#include <sys/resource.h>
#include <sys/time.h>
#endif

#define ERROR_SUCCESS 0
#define DefaultTaskCount 100
#define DefaultTaskProcessTimes 1
#define DefaultThreadNum 1
namespace nett {
struct nett_conf {
  std::string mod = "";
  std::string ip = "127.0.0.1";
  int iothread = 1;
  int tasks = 1;
  int times = 0;
  int port = 8888;
  int echo = 0;
  int dropclient = 0;
};
} // namespace nett

inline uint64_t timenow() {
  auto tm = std::chrono::system_clock::now();
  return tm.time_since_epoch().count();
}
