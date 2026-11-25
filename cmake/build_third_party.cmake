if(CMAKE_SYSTEM_NAME MATCHES Linux)
    add_custom_target(third_party)


    # 要链接的库,也可以用find_library

    message (${CMAKE_CURRENT_BINARY_DIR})

    set(gRPC_DIR ${CMAKE_CURRENT_BINARY_DIR}/third_party/grpc/lib/cmake/grpc)
    set(Protobuf_DIR ${CMAKE_CURRENT_BINARY_DIR}/third_party/protobuf/lib64/cmake/protobuf)
    set(re2_DIR ${CMAKE_CURRENT_BINARY_DIR}/third_party/re2/lib64/cmake/re2)
    set(absl_DIR ${CMAKE_CURRENT_BINARY_DIR}/third_party/absl/lib64/cmake/absl)
    set(c-ares_DIR ${CMAKE_CURRENT_BINARY_DIR}/third_party/c-ares/lib64/cmake/c-ares)

    find_package(gRPC CONFIG REQUIRED)
    find_package(Protobuf CONFIG REQUIRED)

    set(GRPC_CPP_PLUGIN $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

    set(THIRD_PARTY_FRAMEWORK_BASE_LIBRARIES
        libfmt.a
        libjsoncpp.a
        libhiredis.a
        libfmt.a
        libspdlog.a
        liblua.a
        libxml2.a
    )

    set(THIRD_PARTY_FRAMEWORK_DB_LIBRARIES
        libmysqlclient.a
    )

    set(THIRD_PARTY_FRAMEWORK_NET_LIBRARIES
        libcurl.a
    )

    set(THIRD_PARTY_FRAMEWORK_ETCD_LIBRARIES
        protobuf::libprotobuf
        gRPC::grpc++_reflection
        gRPC::grpc++
        libcurl.a
    )

    set(THIRD_PARTY_MESH_SHARED_LIBRARIES
        libyaml-cpp.a
    )

    # 库目录
    list(APPEND THIRD_PARTY_LIB_DIRS 
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    ${CMAKE_BINARY_DIR}/third_party/curl/src/curl/lib/.libs
    ${CMAKE_BINARY_DIR}/third_party/fmt/src/fmt-build
    ${CMAKE_BINARY_DIR}/third_party/jsoncpp/src/jsoncpp-build/lib
    ${CMAKE_BINARY_DIR}/third_party/xml2/src/xml2/.libs
    ${CMAKE_BINARY_DIR}/third_party/lua/src/lua/src
    ${CMAKE_BINARY_DIR}/third_party/spdlog/src/spdlog-build
    ${CMAKE_BINARY_DIR}/third_party/yaml-cpp/src/yaml-cpp-build
    ${CMAKE_BINARY_DIR}/third_party/hiredis/src/hiredis
    ${CMAKE_BINARY_DIR}/third_party/mysql/src/mysql/lib
    )

    # 库头文件
    list(APPEND THIRD_PARTY_INCLUDES
    ${CMAKE_BINARY_DIR}/third_party/concurrentqueue/src/concurrentqueue
    ${CMAKE_BINARY_DIR}/third_party/readerwriterqueue/src/readerwriterqueue
    ${CMAKE_BINARY_DIR}/third_party/curl/src/curl/include
    ${CMAKE_BINARY_DIR}/third_party/fmt/src/fmt/include
    ${CMAKE_BINARY_DIR}/third_party/hiredis/src
    ${CMAKE_BINARY_DIR}/third_party/jsoncpp/src/jsoncpp/include
    ${CMAKE_BINARY_DIR}/third_party/xml2/src/xml2/include
    ${CMAKE_BINARY_DIR}/third_party/lua/src/lua/src
    ${CMAKE_BINARY_DIR}/third_party/spdlog/src/spdlog/include
    ${CMAKE_BINARY_DIR}/third_party/yaml-cpp/src/yaml-cpp/include
    ${CMAKE_BINARY_DIR}/third_party/mysql/src/mysql/include
    ${CMAKE_BINARY_DIR}/third_party/grpc/include
    ${CMAKE_BINARY_DIR}/third_party/absl/include
    ${CMAKE_BINARY_DIR}/third_party/protobuf/include
    ${CMAKE_SOURCE_DIR}/third_party/include
    )

    # $ORIGIN是可执行文件的路径 是推荐的寻找动态库的方式
    # SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    # SET(CMAKE_INSTALL_RPATH $ORIGIN)

    link_directories(${THIRD_PARTY_LIB_DIRS})
    include_directories(${THIRD_PARTY_INCLUDES})
elseif(CMAKE_SYSTEM_NAME MATCHES Windows OR CMAKE_SYSTEM_NAME MATCHES windows)
if(DEFINED ENV{VCPKG_ROOT})
    set(VCPKG_ROOT $ENV{VCPKG_ROOT})
    include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
    set(CMAKE_SYSTEM_NAME_LOWER win)
    set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
    set(VCPKG_TOOLS ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools)
    set(Protobuf_PROTOC_EXECUTABLE ${VCPKG_TOOLS}/protobuf/protoc.exe)
    set(GRPC_CPP_PLUGIN ${VCPKG_TOOLS}/grpc/grpc_cpp_plugin.exe)
    include_directories(${VCPKG_ROOT}/installed/x64-windows/include)
    find_package(fmt CONFIG REQUIRED)
    find_package(spdlog CONFIG REQUIRED)
    find_package(Lua REQUIRED)
    find_package(OpenSSL REQUIRED)
    find_package(PThreads4W REQUIRED)
    find_package(hiredis REQUIRED)
    find_package(Protobuf CONFIG REQUIRED)
    find_package(gRPC CONFIG REQUIRED)
    find_package(CURL CONFIG REQUIRED)
    find_package(jsoncpp CONFIG REQUIRED)
    find_package(libmysql REQUIRED)
    find_package(libmysql REQUIRED)
    find_package(LibXml2 REQUIRED)
else()
    message("Windows 平台 环境变量中找不到 VCPKG_ROOT")
    return()
endif()

if(DEFINED ENV{LLVM_ROOT})
    set(LLVM_ROOT $ENV{LLVM_ROOT})
    set(llvm_include_dir ${LLVM_ROOT}/include)
    set(llvm_libs_dir ${LLVM_ROOT}/libs)
    set(SELECTED_CLANG_LIBS  
    LLVMAggressiveInstCombine 
    LLVMAnalysis
    LLVMAsmParser
    LLVMAsmPrinter
    LLVMBinaryFormat
    LLVMBitReader
    LLVMBitWriter
    LLVMBitstreamReader
    LLVMCFGuard
    LLVMCFIVerify
    LLVMCodeGen
    LLVMCore
    LLVMCoroutines
    LLVMCoverage
    LLVMDWARFLinker
    LLVMDWARFLinkerParallel
    LLVMDWP
    LLVMDebugInfoCodeView
    LLVMDebugInfoDWARF
    LLVMDebugInfoGSYM
    LLVMDebugInfoLogicalView
    LLVMDebugInfoMSF
    LLVMDebugInfoPDB
    LLVMDebuginfod
    LLVMDemangle
    LLVMDiff
    LLVMDlltoolDriver
    LLVMExecutionEngine
    LLVMExegesis
    LLVMExegesisX86
    LLVMExtensions
    LLVMFileCheck
    LLVMFrontendHLSL
    LLVMFrontendOpenACC
    LLVMFrontendOpenMP
    LLVMFuzzMutate
    LLVMFuzzerCLI
    LLVMGlobalISel
    LLVMIRPrinter
    LLVMIRReader
    LLVMInstCombine
    LLVMInstrumentation
    LLVMInterfaceStub
    LLVMInterpreter
    LLVMJITLink
    LLVMLTO
    LLVMLibDriver
    LLVMLineEditor
    LLVMLinker
    LLVMMC
    LLVMMCA
    LLVMMCDisassembler
    LLVMMCJIT
    LLVMMCParser
    LLVMMIRParser
    LLVMObjCARCOpts
    LLVMObjCopy
    LLVMObject
    LLVMObjectYAML
    LLVMOption
    LLVMOrcJIT
    LLVMOrcShared
    LLVMOrcTargetProcess
    LLVMPasses
    LLVMProfileData
    LLVMRemarks
    LLVMRuntimeDyld
    LLVMScalarOpts
    LLVMSelectionDAG
    LLVMSupport
    LLVMSymbolize
    LLVMTableGen
    LLVMTableGenGlobalISel
    LLVMTarget
    LLVMTargetParser
    LLVMTestingAnnotations
    LLVMTestingSupport
    LLVMTextAPI
    LLVMTransformUtils
    LLVMVectorize
    LLVMWindowsDriver
    LLVMWindowsManifest
    LLVMX86AsmParser
    LLVMX86CodeGen
    LLVMX86Desc
    LLVMX86Disassembler
    LLVMX86Info
    LLVMX86TargetMCA
    LLVMXRay
    LLVMipo
    LTO
    Remarks
    clangAPINotes
    clangARCMigrate
    clangAST
    clangASTMatchers
    clangAnalysis
    clangAnalysisFlowSensitive
    clangAnalysisFlowSensitiveModels
    clangBasic
    clangCodeGen
    clangCrossTU
    clangDependencyScanning
    clangDirectoryWatcher
    clangDriver
    clangDynamicASTMatchers
    clangEdit
    clangExtractAPI
    clangFormat
    clangFrontend
    clangFrontendTool
    clangHandleCXX
    clangHandleLLVM
    clangIndex
    clangIndexSerialization
    clangInterpreter
    clangLex
    clangParse
    clangRewrite
    clangRewriteFrontend
    clangSema
    clangSerialization
    clangStaticAnalyzerCheckers
    clangStaticAnalyzerCore
    clangStaticAnalyzerFrontend
    clangSupport
    clangTesting
    clangTooling
    clangToolingASTDiff
    clangToolingCore
    clangToolingInclusions
    clangToolingInclusionsStdlib
    clangToolingRefactoring
    clangToolingSyntax
    clangTransformer
    libclang
    llvm_gtest
    llvm_gtest_main
    version
    )

else()
    message("环境变量中找不到 LLVM_ROOT 依赖llvm的工程将无法编译")
endif()

endif()