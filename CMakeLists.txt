cmake_minimum_required(VERSION 3.1)

option(ENABLE_COVERAGE "Generates the coverage build" OFF)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

project(gamesh_app CXX)
set(CMAKE_CXX_STANDARD 11)

include(${PROJECT_SOURCE_DIR}/cmake/common.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/subdir.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/execute.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/vs_dir_proj.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/proj_directory.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/filter_list.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/use_vcpkg.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/direct_use_vcpkg.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/output_bin.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/coverage.cmake)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    message("------------ Options -------------")
    message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
    message("CMAKE_COMMAND: ${CMAKE_COMMAND}")
    message("CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}")
endif()

message("------------ Options -------------")
message("ENABLE_GAMESH_NET_TEST: ${ENABLE_GAMESH_NET_TEST}")
message("ENABLE_GAMESH_NET_TRAFFIC: ${ENABLE_GAMESH_NET_TRAFFIC}")

include_directories(${PROJECT_SOURCE_DIR}/deps/include)
include_directories(${PROJECT_SOURCE_DIR}/deps/include/${BUILD_OS})
include_directories(${PROJECT_SOURCE_DIR}/modules)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/gamesh/include)
include_directories(${VCPKG_INSTALL}/include)
link_directories(${PROJECT_SOURCE_DIR}/deps/lib_${BUILD_OS}/${BUILD_TYPE_SUFFIX})

add_definitions(${DEFINITIONS})
# set(CMAKE_EXE_LINKER_FLAGS -pg)
# set(CMAKE_MODULE_LINKER_FLAGS -pg)
# set(CMAKE_SHARED_LINKER_FLAGS -pg)
# set(CMAKE_STATIC_LINKER_FLAGS -pg)

enable_testing()

if(ENABLE_COVERAGE)
setup_coverage()
endif()

add_subdirectory(modules)
add_subdirectory(app)
add_subdirectory(test)
