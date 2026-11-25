cmake_minimum_required(VERSION 3.4.1)           # 最小cmake版本
if(CMAKE_SYSTEM_NAME MATCHES Linux)                                        
    # 通用编译参数
    set(CXX_FLAG_LIST
        "-Wno-deprecated"                                                                   # 关闭弃用API的警告
        "-Werror"                                                                           # 视警告为错误;出现任何警告即放弃编译
        "-Wall"                                                                             # 显示所有警告
        "-Wno-unused-function"                                                              # 本地没有使用的函数不警告
        "-Wno-unused-result"                                                                # unused-result 不警告
        "-Wno-misleading-indentation"                                                       # 有歧义的缩进 不警告
        "-D_GNU_SOURCE"                                                                     # 支持对GNU标准
        "-fopenmp" # 启用openMP
        # "--coverage"
        "-Wno-unused-variable"
    )
    string(REPLACE ";" " " CXX_FLAGS "${CXX_FLAG_LIST}")

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # set(CXX_FLAGS "${CXX_FLAGS} -fuse-ld=lld -no-pie -ferror-limit=20 -Wno-unused-command-line-argument")
        set(CXX_FLAGS "${CXX_FLAGS} -fuse-ld=lld -no-pie -ferror-limit=20 -Wno-unused-command-line-argument -Wno-delete-non-abstract-non-virtual-dtor -Wno-inconsistent-missing-override")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CXX_FLAGS "${CXX_FLAGS} -fmax-errors=20 -Wno-maybe-uninitialized -Wno-attributes")
    endif()

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		if(ENABLE_ASAN)
        # 多线程data race 和 undefined 相关 San检测工作集
        # string(APPEND CMAKE_CXX_FLAGS " -fsanitize=thread -fsanitize=undefined -fno-omit-frame-pointer")
		set(CXX_FLAGS "${CXX_FLAGS} -fsanitize=address -fsanitize=leak -fsanitize-recover=address -fno-omit-frame-pointer")
		message(${CXX_FLAGS})
		endif()
    endif()

    set(CMAKE_CXX_FLAGS_DEBUG "${CXX_FLAGS} -O0 -g -ggdb")                            # Debug 参数,日常开发
    set(CMAKE_CXX_FLAGS_RELEASE "${CXX_FLAGS} -O2 -DNDEBUG")                          # Release 参数,上线发布
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CXX_FLAGS} -O2 -g -DNDEBUG")                # RelWithDebInfo 参数,版本发布
    # compiler option
    message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
    message("CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")
    message("CMAKE_CXX_FLAGS_RELWITHDEBINFO is ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    message("CMAKE_CXX_FLAGS_MINSIZEREL is ${CMAKE_CXX_FLAGS_MINSIZEREL}")
elseif(CMAKE_SYSTEM_NAME MATCHES Windows)
    string(APPEND CMAKE_CXX_FLAGS " /MP -wd4251 -wd4275 -wd4819 -wd4200 -DWIN32_LEAN_AND_MEAN -D_WIN64 -DWIN64 -D_WIN32=0x0601 -D_CRT_SECURE_NO_WARNINGS")
    string(APPEND CMAKE_CXX_FLAGS_DEBUG " /MDd")
    string(APPEND CMAKE_CXX_FLAGS_RELEASE " /MD")
    string(APPEND CMAKE_CXX_FLAGS_MINSIZEREL " /MD")
    string(APPEND CMAKE_CXX_FLAGS_RELWITHDEBINFO " /MD")
    add_compile_options("$<$<C_COMPILER_ID:Clang>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:Clang>:/utf-8>")
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        string(APPEND CMAKE_CXX_FLAGS " -fuse-ld=lld -ferror-limit=20 -Wno-unused-command-line-argument -Wno-delete-non-abstract-non-virtual-dtor -Wno-inconsistent-missing-override")
    endif()
endif()
