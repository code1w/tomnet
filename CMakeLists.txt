cmake_minimum_required(VERSION 2.8.7)
set(CMAKE_MACOSX_RPATH 1)
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

project(tomnet CXX)

enable_testing()
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

include(${PROJECT_SOURCE_DIR}/cmake/build_config.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/tomvcpkg.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/vs_dir_proj.cmake)


if(CMAKE_PROJECT_NAME STREQUAL "tomnet")
	option(TOMNET_BUILD_EXAMPLES "Build Tomnet examples" ON)
endif()

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/3rd/)
include_directories(${PROJECT_SOURCE_DIR}/tomnet/)


if(TOMNET_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif() 

add_subdirectory(tomnet/net)
add_subdirectory(tomnet/base)


