if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(BUILD_OS linux)

    if("${CMAKE_BUILD_TYPE}" STREQUAL "")
        set(CMAKE_BUILD_TYPE debug)
    endif()

    string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
    if(CMAKE_BUILD_TYPE MATCHES RELEASE)
        set(BUILD_TYPE_SUFFIX release)
    elseif(CMAKE_BUILD_TYPE MATCHES RELWITHDEBINFO)
        set(BUILD_TYPE_SUFFIX release)
    elseif(CMAKE_BUILD_TYPE MATCHES MINSIZEREL)
        set(BUILD_TYPE_SUFFIX release)
    else()
        set(CMAKE_BUILD_TYPE DEBUG)
        set(BUILD_TYPE_SUFFIX debug)
    endif()

    set(DEFINITIONS -fPIC)
    # set(DEFINITIONS -fPIC -pg)
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")

    set(BOST_D_BUILD_SUFFIX)
    set(BOST_BUILD_SUFFIX)
    set(LIB_PREFIX)
    set(THREAD pthread)
    set(VCPKG_INSTALL ${PROJECT_SOURCE_DIR}/vcpkg/installed/x64-linux)

    set(PLAT_LIBS dl pthread aprutil-1 apr-1)
else()
    set(BUILD_OS win)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MD")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MD")

    set(DEFINITIONS -DWIN32_LEAN_AND_MEAN -D_WIN32=0x0601 -D_CRT_SECURE_NO_WARNINGS -DPROTOBUF_USE_DLLS)

    set(BOST_D_BUILD_SUFFIX -vc140-mt-gd)
    set(BOST_BUILD_SUFFIX -vc140-mt)
    set(LIB_PREFIX lib)
    set(THREAD)
    set(VCPKG_INSTALL ${PROJECT_SOURCE_DIR}/vcpkg/installed/x64-windows)

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

set(
    PB_LIBS
    protobuf::libprotobuf
    protobuf::libprotoc
)

set(
    ASIO_LIBS
    debug boost_coroutine${BOST_D_BUILD_SUFFIX}
    debug boost_thread${BOST_D_BUILD_SUFFIX}
    debug boost_system${BOST_D_BUILD_SUFFIX}
    debug boost_context${BOST_D_BUILD_SUFFIX}
    debug boost_date_time${BOST_D_BUILD_SUFFIX}
    debug boost_chrono${BOST_D_BUILD_SUFFIX}
    debug boost_filesystem${BOST_D_BUILD_SUFFIX}
    debug boost_regex${BOST_D_BUILD_SUFFIX}
    debug boost_atomic${BOST_D_BUILD_SUFFIX}
    optimized boost_coroutine${BOST_BUILD_SUFFIX}
    optimized boost_thread${BOST_BUILD_SUFFIX}
    optimized boost_system${BOST_BUILD_SUFFIX}
    optimized boost_context${BOST_BUILD_SUFFIX}
    optimized boost_date_time${BOST_BUILD_SUFFIX}
    optimized boost_chrono${BOST_BUILD_SUFFIX}
    optimized boost_filesystem${BOST_BUILD_SUFFIX}
    optimized boost_regex${BOST_BUILD_SUFFIX}
    optimized boost_atomic${BOST_BUILD_SUFFIX}
    ${LIB_PREFIX}ssl
)

set(
    COMMON_LIBS
    ${PLAT_LIBS}
    log4cxx
    ${PB_LIBS}
)

set(
    SVR_COMMON_LIBS
    ${COMMON_LIBS}
    ${ASIO_LIBS}
    libgo
)

set(
    NATS
    debug natsd
    debug natsd
    optimized nats
    optimized nats
)

set(
    GTEST
    debug gtestd
    debug gmockd
    optimized gtest
    optimized gmock
    ${THREAD}
)
