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

if(WIN32)
  add_definitions(-DTOM_TOM_NET_DLL_EXPORT)
else()
  set(CMAKE_CXX_FLAGS 
  ${CMAKE_CXX_FLAGS} 
  -g
  # -DVALGRIND
  -DCHECK_PTHREAD_RETURN_VALUE
  -D_FILE_OFFSET_BITS=64
  -Wall
  -Wextra
  -Werror
  -Wconversion
  -Wno-unused-parameter
  -Wold-style-cast
  -Woverloaded-virtual
  -Wpointer-arith
  -Wshadow
  -Wwrite-strings
  -march=native
  # -MMD
  -std=c++11
  -rdynamic
  )

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  list(APPEND CXX_FLAGS "-Wno-null-dereference")
  list(APPEND CXX_FLAGS "-Wno-sign-conversion")
  list(APPEND CXX_FLAGS "-Wno-unused-local-typedef")
  list(APPEND CXX_FLAGS "-Wthread-safety")
  list(REMOVE_ITEM CXX_FLAGS "-rdynamic")
endif()

endif(WIN32)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/3rd/)
include_directories(${PROJECT_SOURCE_DIR}/tomnet/)


if(TOMNET_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif() 

add_subdirectory(tomnet/net)
add_subdirectory(tomnet/base)


