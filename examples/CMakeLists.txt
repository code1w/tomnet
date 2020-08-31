cmake_minimum_required(VERSION 2.8.7)
set(projectName tomnet_test)
file(GLOB_RECURSE HEADER_LIST *.h *.inl)
file(GLOB_RECURSE SRC_LIST *.cpp *.c *.cc *.cxx)
set(ALL_FILES ${HEADER_LIST} ${SRC_LIST})
source_group_by_dir("${ALL_FILES}")
add_executable(${projectName} ${ALL_FILES})
target_link_libraries(
	${projectName}
	libprotobuf
    tomnet
)

