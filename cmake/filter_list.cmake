# filterList(newlist pattern [INCLUDE|EXCLUDE] [item1] [item2...])
function(filterList newlist pattern)
    set(exclude NO)
    set(i 0)
    if("${ARGV2}" STREQUAL "INCLUDE")
        set(exclude NO)
        set(i 1)
    elseif("${ARGV2}" STREQUAL "EXCLUDE")
        set(exclude YES)
        set(i 1)
    endif()

    list(LENGTH ARGN length)
    while(i LESS ${length})
        list(GET ARGN ${i} file)
        math(EXPR i "${i}+1")
        # message("check ${file}")

        if(${file} MATCHES ${pattern})
            # message("matched")
            if("${exclude}" STREQUAL "NO")
                list(APPEND ret ${file})
                # message("add file ${file}")
            endif()
        else()
            # message("not matched")
            if("${exclude}" STREQUAL "YES")
                list(APPEND ret ${file})
                # message("add file ${file}")
            endif()
        endif()
    endwhile()
    set(${newlist} ${ret} PARENT_SCOPE)
endfunction(filterList)