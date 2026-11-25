cmake_minimum_required(VERSION 2.8.7)
set(CMAKE_MACOSX_RPATH 1)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")                              # 设置cmake模块路径

project(tomnet)
set(CMAKE_CXX_STANDARD 20)   
enable_testing()
include(compiler)
include(build_third_party)
include(build_config)
#include(${PROJECT_SOURCE_DIR}/cmake/tomvcpkg.cmake)
include(vs_dir_proj)


if(CMAKE_PROJECT_NAME STREQUAL "tomnet")
	option(TOMNET_BUILD_EXAMPLES "Build Tomnet examples" ON)
	option(TOMNET_BUILD_BENCHMARK "Build Tomnet benchmark" ON)
endif()

if(NOT WIN32)
execute_process(COMMAND bash ../3rd/3rd-build.sh)
endif()

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/3rd/)
include_directories(${PROJECT_SOURCE_DIR}/3rd/asio/)
include_directories(${PROJECT_SOURCE_DIR}/tomnet/)
if(NOT WIN32)
include_directories(${PROJECT_SOURCE_DIR}/3rd/${jemalloc}/include/)
include_directories(${PROJECT_SOURCE_DIR}/3rd/${protobuf}/src/)
endif()


if(TOMNET_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(TOMNET_BUILD_BENCHMARK)
  add_subdirectory(benchmark)
endif()

add_subdirectory(tomnet/net)
add_subdirectory(tomnet/base)

message(cxx_flags =  "  ${CMAKE_CXX_FLAGS}")
