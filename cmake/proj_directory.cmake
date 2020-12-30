function(proj_directory name)
    if("${CMAKE_CURRENT_SOURCE_DIR}" MATCHES "${CMAKE_SOURCE_DIR}/modules/.+")
        set_property(TARGET ${name} PROPERTY FOLDER "Modules")
    endif()

    if("${CMAKE_CURRENT_SOURCE_DIR}" MATCHES "${CMAKE_SOURCE_DIR}/app/.+")
        set_property(TARGET ${name} PROPERTY FOLDER "App")
    endif()

    if("${CMAKE_CURRENT_SOURCE_DIR}" MATCHES "^${CMAKE_SOURCE_DIR}/.+/tests$")
        set_property(TARGET ${name} PROPERTY FOLDER "Tests")
    endif()

    if("${CMAKE_CURRENT_SOURCE_DIR}" MATCHES "^${CMAKE_SOURCE_DIR}/test")
        set_property(TARGET ${name} PROPERTY FOLDER "Tests")
    endif()
endfunction(proj_directory)