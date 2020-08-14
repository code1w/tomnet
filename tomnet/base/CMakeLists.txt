cmake_minimum_required(VERSION 3.1)
project(common CXX)

file(GLOB_RECURSE HEADER_LIST *.h *.inl)
file(GLOB_RECURSE SRC_LIST *.cpp *.c *.cc *.cxx)

set(ALL_FILES ${HEADER_LIST} ${SRC_LIST})
source_group_by_dir("${ALL_FILES}")

direct_use_vcpkg()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

#add_definitions(-DKERNEL_DLL_EXPORT)
add_library(common STATIC ${ALL_FILES})
proj_directory(common)
target_link_libraries(
    common
    jsoncpp_lib
    CURL::libcurl
    OpenSSL::Crypto
)
