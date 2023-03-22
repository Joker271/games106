function (link_directory_command OUTPUT SRC DST)
    if (CMAKE_HOST_WIN32)
        file(TO_NATIVE_PATH "${SRC}" SRC)
        file(TO_NATIVE_PATH "${DST}" DST)
        set(${OUTPUT} COMMAND cmd /c mklink /j "${DST}" "${SRC}" PARENT_SCOPE)
    else ()
        message(FATAL_ERROR "Only build in win32")
    endif ()
endfunction ()

function (remove_directory_command OUTPUT DIR)
    if (CMAKE_HOST_WIN32)
        file(TO_NATIVE_PATH "${DIR}" DIR)
        set(${OUTPUT}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DIR}"
            COMMAND cmd /c rmdir /s /q "${DIR}"
            PARENT_SCOPE
        )
    else ()
        message(FATAL_ERROR "Only build in win32")
    endif ()
endfunction ()
