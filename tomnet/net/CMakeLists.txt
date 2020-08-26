cmake_minimum_required(VERSION 2.8.7)
project(libtomnet CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g")
file(ALL_FILES  *.h *.inl *.cpp *.c *.cc *.cxx)

#option(ENABLE_ TOM_ TOM_NET_TEST "enable gamesh net test" OFF)
#if(ENABLE_ TOM_ TOM_NET_TEST)
#	add_definitions(-D TOM_ TOM_NET_TEST)
#endif()

#option(ENABLE_ TOM_ TOM_NET_TRAFFIC "enable gamesh net traffic" OFF)
#if(ENABLE_ TOM_ TOM_NET_TRAFFIC)
#	add_definitions(-D TOM_ TOM_NET_TRAFFIC)
#endif()

#option(ENABLE_ TOM_ TOM_NET_POLL_TEST "enable gamesh net poll test" OFF)
#if(ENABLE_ TOM_ TOM_NET_TRAFFIC)
#	add_definitions(-D TOM_ TOM_NET_POLL_TEST)
#endif()

#if(CMAKE_SYSTEM_NAME MATCHES "Linux")
#    message("------------gamesh_net Options -------------")
#    message("CMAKE_CXX_FLAGS : ${CMAKE_CXX_FLAGS}")
#endif()

source_group_by_dir("${ALL_FILES}")

add_definitions(-D TOM_ TOM_NET_DLL_EXPORT)
add_library(gamesh_net SHARED ${ALL_FILES})
proj_directory(gamesh_net)
add_dependencies(gamesh_net gamesh)

set_target_properties(gamesh_net PROPERTIES
    CXX_STANDARD 17
)
target_link_libraries(gamesh_net ${PLAT_LIBS} gamesh)
