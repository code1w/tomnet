// loopback echo 测试 - 客户端进程
// 每个连接维持 window 条未收到回显的消息在途, 收到即补发;
// 每秒打印回显吞吐、逻辑线程 CPU、队列积压, 结束时打印 SUMMARY
//
// 用法: tomnet_bench_client [clients] [msg_size] [window] [seconds]
//                           [iothreads] [port] [spin] [server_ip]
//   clients   连接数              默认 4
//   msg_size  消息体大小(字节)     默认 4096
//   window    每连接在途消息数     默认 16
//   seconds   采样时长(秒)        默认 10
//   iothreads IO 线程数           默认 4
//   port      服务端端口           默认 19888
//   spin      1=逻辑线程忙轮询 0=空闲休眠   默认 1
//   server_ip 服务端地址           默认 127.0.0.1
#include "bench_common.h"
#include "net/tomnet.h"
#include "base/buffer.h"
#include "net/message_queue.h"

#include <atomic>
#include <map>
#include <string>

using namespace tom::net;
using namespace tombench;

static std::atomic<uint64_t> g_echo_bytes{0};
static std::atomic<uint64_t> g_connect_ok{0};
static std::atomic<ThreadId> g_logic_tid{0};

int main(int argc, char** argv) {
  int clients = argc > 1 ? atoi(argv[1]) : 4;
  int msgsize = argc > 2 ? atoi(argv[2]) : 4096;
  int window = argc > 3 ? atoi(argv[3]) : 16;
  int seconds = argc > 4 ? atoi(argv[4]) : 10;
  int iothreads = argc > 5 ? atoi(argv[5]) : 4;
  int port = argc > 6 ? atoi(argv[6]) : 19888;
  g_spin = argc > 7 ? atoi(argv[7]) : 1;
  const char* ip = argc > 8 ? argv[8] : "127.0.0.1";

  // 预先编好的整包: | int32 len | payload |
  std::string payload(msgsize, 'a');
  tom::Buffer packet;
  packet.appendInt32(msgsize);
  packet.append(payload.data(), payload.size());

  CreateNetwork(asio);
  InitNetwork(iothreads);

  IMessageQueue* cq = nullptr;
  for (int i = 0; i < clients; i++) Connect(ip, port, &cq, 0, 0, nametype, nullptr);

  std::atomic<bool> stop{false};

  // 逻辑: 每连接维持 window 条在途
  std::thread logic([&] {
    g_logic_tid = CurrentThreadId();
    std::map<uint32_t, int> outstanding;
    while (!stop) {
      bool idle = true;
      while (auto ev = cq->PopMessage()) {
        idle = false;
        auto* ctx = (NetContext*)ev->peek();
        uint32_t h = ctx->handler_;
        switch (ctx->evetype_) {
          case EVENT_CONNECT_SUCCESS:
            outstanding[h] = 0;
            g_connect_ok++;
            break;
          case EVENT_NETMSG:
            g_echo_bytes += ev->readableBytes() - sizeof(NetContext) - 4;
            if (outstanding.count(h)) outstanding[h]--;
            break;
        }
      }
      for (auto& kv : outstanding) {
        while (kv.second < window) {
          SendPacket(kv.first, packet.peek(), (uint32_t)packet.readableBytes());
          kv.second++;
        }
      }
      LogicThreadIdle(idle);
    }
  });

  // 等待全部连接建立
  for (int i = 0; i < 1000 && g_connect_ok.load() < (uint64_t)clients; i++)
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  if (g_connect_ok.load() < (uint64_t)clients) {
    printf("connect %s:%d failed, %lu/%d connected\n", ip, port,
           (unsigned long)g_connect_ok.load(), clients);
    stop = true;
    logic.join();
    ExitProcessNow();
  }

  printf("clients=%d msgsize=%d window=%d iothreads=%d spin=%d server=%s:%d\n",
         clients, msgsize, window, iothreads, g_spin, ip, port);
  fflush(stdout);

  // 预热
  std::this_thread::sleep_for(std::chrono::seconds(2));
  uint64_t r0 = g_echo_bytes.load();
  auto t0 = std::chrono::steady_clock::now();

  uint64_t last_b = r0;
  int64_t last_t = ReadThreadCpuUs(g_logic_tid.load());
  auto last_w = t0;
  for (int s = 1; s <= seconds; s++) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto now = std::chrono::steady_clock::now();
    double dt = std::chrono::duration<double>(now - last_w).count();
    uint64_t b = g_echo_bytes.load();
    int64_t t = ReadThreadCpuUs(g_logic_tid.load());
    double cpu = t >= 0 && last_t >= 0 ? (t - last_t) / 1e6 / dt * 100.0 : -1;
    auto* mq = static_cast<tom::net::MessageQueue*>(cq);
    printf("  t=%3ds 回显 %7.1f MiB/s (%6.0fk msg/s)  逻辑CPU %5.1f%%  积压 %zu\n",
           s, (b - last_b) / dt / 1048576.0, (b - last_b) / dt / (msgsize + 4) / 1000.0,
           cpu, mq->Size());
    fflush(stdout);
    last_b = b;
    last_t = t;
    last_w = now;
  }

  auto t1 = std::chrono::steady_clock::now();
  double total_sec = std::chrono::duration<double>(t1 - t0).count();
  uint64_t rb = g_echo_bytes.load() - r0;
  printf("SUMMARY clients=%d msgsize=%d window=%d iothreads=%d  回显 %.1f MiB/s  "
         "(%.0fk msg/s)  (%.6f S)\n",
         clients, msgsize, window, iothreads, rb / total_sec / 1048576.0,
         rb / total_sec / (msgsize + 4) / 1000.0, total_sec);
  fflush(stdout);
  stop = true;
  logic.join();
  ExitProcessNow();
}
