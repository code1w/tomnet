function(direct_use_vcpkg)
	if(CMAKE_SYSTEM_NAME MATCHES "Linux")
		if(${BUILD_TYPE_SUFFIX} MATCHES debug)
			link_directories(${VCPKG_INSTALL}/debug/lib/)
		else()
			link_directories(${VCPKG_INSTALL}/lib)
		endif()
	else()
		link_directories(${VCPKG_INSTALL}/$(Configuration)/lib/)
		link_directories(${VCPKG_INSTALL}/lib)
	endif()
endfunction(direct_use_vcpkg)