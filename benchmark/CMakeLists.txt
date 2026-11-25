cmake_minimum_required(VERSION 3.5)

# 拆分为两个进程: 服务端 / 客户端
add_executable(tomnet_bench_server bench_server.cc)
target_link_libraries(tomnet_bench_server tomnet)

add_executable(tomnet_bench_client bench_client.cc)
target_link_libraries(tomnet_bench_client tomnet)

if(NOT WIN32)
    target_link_libraries(tomnet_bench_server pthread)
    target_link_libraries(tomnet_bench_client pthread)
endif()
