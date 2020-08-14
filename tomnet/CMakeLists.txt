subdirlist(modules ${CMAKE_CURRENT_SOURCE_DIR})

foreach(module ${modules})
    add_subdirectory(${module})
endforeach()