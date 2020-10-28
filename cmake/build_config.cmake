if(NOT WIN32)
    set(BUILD_OS linux)

    if("${CMAKE_BUILD_TYPE}" STREQUAL "")
        set(CMAKE_BUILD_TYPE debug)
    endif()

   message("${CMAKE_BUILD_TYPE}")
    if(CMAKE_BUILD_TYPE MATCHES "release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
    endif()
    set(DEFINITIONS -fPIC)
    set(PLAT_LIBS dl pthread aprutil-1 apr-1)
    message(cxx_flags =  "  ${CMAKE_CXX_FLAGS}")
else()
    set(BUILD_OS win)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MD")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MD")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MD")
    set(DEFINITIONS -DWIN32_LEAN_AND_MEAN -D_WIN32=0x0601 -D_CRT_SECURE_NO_WARNINGS -DPROTOBUF_USE_DLLS)
    set(PLAT_LIBS ws2_32)
    add_definitions(-wd4251)
    add_definitions(-wd4275)
    add_definitions(-wd4819)
endif()

set(BIN_OUTPUT ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG            ${BIN_OUTPUT}/debug/exe)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE          ${BIN_OUTPUT}/release/exe)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO   ${BIN_OUTPUT}/release/exe)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL       ${BIN_OUTPUT}/release/exe)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG            ${BIN_OUTPUT}/debug/dll)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE          ${BIN_OUTPUT}/release/dll)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO   ${BIN_OUTPUT}/release/dll)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL       ${BIN_OUTPUT}/release/dll)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG            ${BIN_OUTPUT}/debug/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE          ${BIN_OUTPUT}/release/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO   ${BIN_OUTPUT}/release/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL       ${BIN_OUTPUT}/release/lib)

