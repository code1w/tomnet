#ifndef BASE_SYSTEM_H_
#define BASE_SYSTEM_H_
#include <string>
#include <string_view>
#include <vector>
#if defined(linux) || defined(__linux) || defined(__linux__)
#include <arpa/inet.h>  // for ntohl()
#include <errno.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
typedef int fd_t;
#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif
#elif defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#define __windows__
#include <WinSock2.h>
#include <Windows.h>

#include <DbgHelp.h>
#include <crtdbg.h>
#include <fcntl.h>
#include <io.h>
#include <iphlpapi.h>
#include <process.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <winnt.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "DbgHelp.lib")

#ifndef TEMP_FAILURE_RETRY
#define TEMP_FAILURE_RETRY
#endif

#ifndef __PRETTY_FUNCTION__
#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#else
#define __PRETTY_FUNCTION__ __PRETTY_FUNCTION__
#endif
#endif

#ifndef AF_LOCAL
#define AF_LOCAL AF_UNIX
#endif

#ifndef ssize_t
#define ssize_t intptr_t
#endif

#ifndef socklen_t
#define socklen_t int
#endif

#ifndef in_addr_t
#define in_addr_t ULONG
#endif

#ifndef SHUT_WR
#define SHUT_WR SD_SEND
#endif

#undef max
#undef min

typedef SOCKET fd_t;
#define __TACOPIE_INVALID_FD INVALID_SOCKET

#endif
namespace base {
void SetCurrentConsole();
#ifdef __linux__
void CreateProcess(std::string_view appname, char* const* argv);
#endif
void GetCurrentProcessPath(std::string& exedir, std::string& exename);
#ifdef __windows__
void CreateProcess(std::string_view appname, const std::string& argv);
void CreateDumpFile(const char* lpstrDumpFilePathName, EXCEPTION_POINTERS* pException);
LONG ApplicationCrashHandler(EXCEPTION_POINTERS* pException);
#endif
}  // namespace base
#endif
