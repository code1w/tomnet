cmake_minimum_required(VERSION 2.8.7)
file(GLOB_RECURSE SRCS_LIST  *.h *.inl *.cpp *.c *.cc *.cxx)
set(ALL_FILES STATIC ${SRCS_LIST})
add_library(tomnet_base ${ALL_FILES})

