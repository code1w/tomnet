cmake_minimum_required(VERSION 3.1)
set(projectName net_test)
#project(net_test CXX)

#include_directories(${PROJECT_SOURCE_DIR})

# ---- Add source files ----
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g")
file(GLOB_RECURSE HEADER_LIST *.h *.inl)
file(GLOB_RECURSE SRC_LIST *.cpp *.c *.cc *.cxx)

set(ALL_FILES ${HEADER_LIST} ${SRC_LIST})
source_group_by_dir("${ALL_FILES}")

add_executable(${projectName} ${ALL_FILES})
proj_directory(${projectName})
#add_dependencies(net_test gamesh_net)
target_link_libraries(
	${projectName}
	${PLAT_LIBS}
	protobuf::libprotobuf
    gamesh_net
)

