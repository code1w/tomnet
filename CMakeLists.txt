cmake_minimum_required(VERSION 2.8.7)
set(CMAKE_MACOSX_RPATH 1)
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

project(tomnet CXX)

enable_testing()

include(${PROJECT_SOURCE_DIR}/cmake/build_config.cmake)
#include(${PROJECT_SOURCE_DIR}/cmake/tomvcpkg.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/vs_dir_proj.cmake)


if(CMAKE_PROJECT_NAME STREQUAL "tomnet")
	option(TOMNET_BUILD_EXAMPLES "Build Tomnet examples" ON)
endif()

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/3rd/)
include_directories(${PROJECT_SOURCE_DIR}/3rd/asio/)
include_directories(${PROJECT_SOURCE_DIR}/tomnet/)

if(NOT WIN32)
include_directories(${PROJECT_SOURCE_DIR}/3rd/jemalloc/include/)
include_directories(${PROJECT_SOURCE_DIR}/3rd/tbb/)
endif()



if(NOT WIN32)
link_directories(${PROJECT_SOURCE_DIR}/3rd/libs/x64-linux/)
link_directories(${PROJECT_SOURCE_DIR}/3rd/jemalloc/lib/)
else()
link_directories(${PROJECT_SOURCE_DIR}/3rd/libs/x64-windows/)
endif()



if(TOMNET_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif() 

add_subdirectory(tomnet/net)
add_subdirectory(tomnet/base)

message(cxx_flags =  "  ${CMAKE_CXX_FLAGS}")
