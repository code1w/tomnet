function(vs_proj_debug)
    if(BUILD_OS MATCHES win)
        set(NODE "${CMAKE_SOURCE_DIR}/tools/node/node.exe")
        gamesh_execute(COMMAND "${NODE}" "${CMAKE_SOURCE_DIR}/tools/node_scripts/vs_proj_debug.js" )
    endif()
endfunction(vs_proj_debug)