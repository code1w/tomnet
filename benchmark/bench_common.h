// bench 客户端/服务端共用的平台辅助代码
#pragma once

#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/syscall.h>
#include <unistd.h>
#endif

namespace tombench {

#ifdef _WIN32
using ThreadId = DWORD;

inline ThreadId CurrentThreadId() { return GetCurrentThreadId(); }

// 返回线程 CPU 时间(微秒, user + kernel), 失败返回 -1
inline int64_t ReadThreadCpuUs(ThreadId tid) {
  HANDLE h = OpenThread(THREAD_QUERY_INFORMATION, FALSE, tid);
  if (!h) h = OpenThread(THREAD_QUERY_LIMITED_INFORMATION, FALSE, tid);
  if (!h) return -1;
  FILETIME create, exit_t, kernel, user;
  BOOL ok = GetThreadTimes(h, &create, &exit_t, &kernel, &user);
  CloseHandle(h);
  if (!ok) return -1;
  auto ft2us = [](const FILETIME& ft) -> int64_t {
    ULARGE_INTEGER v;
    v.LowPart = ft.dwLowDateTime;
    v.HighPart = ft.dwHighDateTime;
    return (int64_t)(v.QuadPart / 10);  // 100ns -> us
  };
  return ft2us(kernel) + ft2us(user);
}
#else
using ThreadId = int;

inline ThreadId CurrentThreadId() { return (ThreadId)syscall(SYS_gettid); }

// 返回线程 CPU 时间(微秒, user + kernel), 失败返回 -1
inline int64_t ReadThreadCpuUs(ThreadId tid) {
  char path[64], buf[1024];
  snprintf(path, sizeof path, "/proc/self/task/%d/stat", tid);
  int fd = open(path, O_RDONLY);
  if (fd < 0) return -1;
  int n = read(fd, buf, sizeof buf - 1);
  close(fd);
  if (n <= 0) return -1;
  buf[n] = 0;
  char* p = strrchr(buf, ')');
  if (!p) return -1;
  long vals[15] = {0};
  int idx = 0;
  char* tok = strtok(p + 1, " ");
  while (tok && idx < 15) { vals[idx++] = atol(tok); tok = strtok(nullptr, " "); }
  // ")" 之后第一个是 state(字段3), 所以 utime=vals[11], stime=vals[12], 单位 jiffies
  long hz = sysconf(_SC_CLK_TCK);
  if (hz <= 0) hz = 100;
  return (int64_t)(vals[11] + vals[12]) * 1000000 / hz;
}
#endif

inline int g_spin = 1;  // 1=逻辑线程忙轮询(压到极限) 0=空闲休眠

inline void LogicThreadIdle(bool idle) {
  if (!idle) return;
  if (g_spin)
    std::this_thread::yield();
  else
    std::this_thread::sleep_for(std::chrono::microseconds(50));
}

[[noreturn]] inline void ExitProcessNow() {
#ifdef _WIN32
  ExitProcess(0);
#else
  _exit(0);
#endif
}

}  // namespace tombench
