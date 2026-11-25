// loopback echo 测试 - 服务端进程
// 收到消息原样回显; 每秒打印吞吐、逻辑线程 CPU、队列积压
//
// 用法: tomnet_bench_server [seconds] [iothreads] [port] [spin]
//   seconds   运行时长(秒), 0=一直运行直到 Ctrl+C   默认 0
//   iothreads IO 线程数                             默认 4
//   port      监听端口                               默认 19888
//   spin      1=逻辑线程忙轮询 0=空闲休眠             默认 1
#include "bench_common.h"
#include "net/tomnet.h"
#include "net/message_queue.h"

#include <atomic>

using namespace tom::net;
using namespace tombench;

static std::atomic<uint64_t> g_recv_bytes{0};
static std::atomic<uint64_t> g_recv_msgs{0};
static std::atomic<uint64_t> g_conns{0};
static std::atomic<ThreadId> g_logic_tid{0};

int main(int argc, char** argv) {
  int seconds = argc > 1 ? atoi(argv[1]) : 0;
  int iothreads = argc > 2 ? atoi(argv[2]) : 4;
  int port = argc > 3 ? atoi(argv[3]) : 19888;
  g_spin = argc > 4 ? atoi(argv[4]) : 1;

  CreateNetwork(asio);
  InitNetwork(iothreads);

  IMessageQueue* sq = nullptr;
  if (!StartNetService("127.0.0.1", port, &sq, 0, 0, 100, nametype, nullptr)) {
    printf("start server failed, port %d in use?\n", port);
    return 1;
  }
  printf("server listening 127.0.0.1:%d  iothreads=%d spin=%d\n", port, iothreads,
         g_spin);
  fflush(stdout);

  std::atomic<bool> stop{false};

  // 逻辑: 收到即回显
  std::thread logic([&] {
    g_logic_tid = CurrentThreadId();
    while (!stop) {
      bool idle = true;
      while (auto ev = sq->PopMessage()) {
        idle = false;
        auto* ctx = (NetContext*)ev->peek();
        uint32_t h = ctx->handler_;
        switch (ctx->evetype_) {
          case EVENT_ACCEPT:
            g_conns++;
            LinkReady(h, nullptr);
            break;
          case EVENT_CLOSE:
            g_conns--;
            SetUserData(h, nullptr);
            break;
          case EVENT_NETMSG: {
            ev->retrieve(sizeof(NetContext));
            g_recv_bytes += ev->readableBytes();
            g_recv_msgs++;
            SendPacket(h, ev->peek(), ev->readableBytes());
          } break;
        }
      }
      LogicThreadIdle(idle);
    }
  });

  // 每秒统计
  uint64_t last_b = 0, last_m = 0;
  int64_t last_t = ReadThreadCpuUs(g_logic_tid.load());
  auto last_w = std::chrono::steady_clock::now();
  for (int s = 1; seconds <= 0 || s <= seconds; s++) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto now = std::chrono::steady_clock::now();
    double dt = std::chrono::duration<double>(now - last_w).count();
    uint64_t b = g_recv_bytes.load(), m = g_recv_msgs.load();
    int64_t t = ReadThreadCpuUs(g_logic_tid.load());
    double cpu = t >= 0 && last_t >= 0 ? (t - last_t) / 1e6 / dt * 100.0 : -1;
    auto* mq = static_cast<tom::net::MessageQueue*>(sq);
    printf("  t=%4ds 收 %8.1f MiB/s (%6.0fk msg/s)  逻辑CPU %5.1f%%  积压 %zu  连接 %lu\n",
           s, (b - last_b) / dt / 1048576.0, (m - last_m) / dt / 1000.0, cpu,
           mq->Size(), (unsigned long)g_conns.load());
    fflush(stdout);
    last_b = b;
    last_m = m;
    last_t = t;
    last_w = now;
  }

  stop = true;
  logic.join();
  ExitProcessNow();
}
