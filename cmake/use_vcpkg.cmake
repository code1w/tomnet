find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(msgpack CONFIG REQUIRED)
find_package(unofficial-libuv CONFIG REQUIRED)
find_package(oatpp CONFIG REQUIRED)
find_package(oatpp-consul CONFIG REQUIRED)
find_package(unofficial-iniparser CONFIG REQUIRED)
include(FindLua)
# find_package(lua REQUIRED)

include_directories(${oatpp_INCLUDE_DIRS})
include_directories(${oatpp-consul_INCLUDE_DIRS})
include_directories(${VCPKG_INSTALL}/include)
