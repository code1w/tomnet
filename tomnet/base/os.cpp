#include "os.h"

#include <iostream>

namespace base {
#ifdef __windows__
void CreateDumpFile(const char *lpstrDumpFilePathName,
                    EXCEPTION_POINTERS *pException) {
  HANDLE hDumpFile = CreateFileA(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL,
                                 CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
  dumpInfo.ExceptionPointers = pException;
  dumpInfo.ThreadId = GetCurrentThreadId();
  dumpInfo.ClientPointers = TRUE;
  MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile,
                    MiniDumpNormal, &dumpInfo, NULL, NULL);
  CloseHandle(hDumpFile);
}

LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException) {
  char buffer[MAX_PATH];
  GetModuleFileName(NULL, buffer, MAX_PATH);
  std::string exePath(buffer);
  std::string exeDir = exePath.substr(0, exePath.find_last_of('.'));
  std::string exeName = exeDir.substr(exePath.find_last_of('\\') + 1);
  std::string dumpname =
      exeDir + "-" + std::to_string((int32_t)GetCurrentProcessId()) + ".dmp";
  char hostname[256];
  {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
      return EXCEPTION_EXECUTE_HANDLER;
    }
    gethostname(hostname, sizeof(hostname));
  }
  std::string strHostName(hostname);
  CreateDumpFile(dumpname.c_str(), pException);
  return EXCEPTION_EXECUTE_HANDLER;
}
#endif

void SetCurrentConsole() {
#ifdef __windows__
  system("chcp 65001"); // 使用 65001 代码页（UTF-8）
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // 获取标准输出句柄
  CONSOLE_FONT_INFOEX fontInfo;
  fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
  GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo); // 获取当前字体信息

  // 修改字体属性
  fontInfo.dwFontSize.X = 20;               // 字体宽度
  fontInfo.dwFontSize.Y = 20;               // 字体高度
  fontInfo.FontWeight = FW_NORMAL;          // 字体粗细
  wcscpy_s(fontInfo.FaceName, L"Consolas"); // 字体名称
  SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

  // windows socket
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    std::cerr << "Failed to initialize Windows Sockets." << std::endl;
    exit(0);
  }
  SymInitialize(GetCurrentProcess(), NULL, TRUE);
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
  SetUnhandledExceptionFilter(
      (LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
#endif
}

#ifdef __linux__
void CreateProcess(std::string_view appname, char *const *argv) {
  pid_t pid = ::fork();
  if (pid == 0) {
    if (::execvp(std::string(appname).c_str(), &argv[0]) < 0) {
      std::cerr << "启动子进程失败 errcode : " << strerror(errno) << std::endl;
      ::exit(0);
    }
  }
}
#endif

#ifdef __windows__
void CreateProcess(std::string_view appname, const std::string &argv) {
  STARTUPINFO startupinfo;
  PROCESS_INFORMATION processinfo;
  ZeroMemory(&startupinfo, sizeof(startupinfo));
  startupinfo.cb = sizeof(startupinfo);
  std::cout << "master cmdline : " << argv << std::endl;
  if (!CreateProcessA((LPSTR)std::string(appname).c_str(), (LPSTR)argv.c_str(),
                      NULL, NULL, FALSE, NULL, NULL, NULL, &startupinfo,
                      &processinfo)) {
    std::cerr << "启动子进程失败" << std::endl;
  }
}
#endif

} // namespace base
