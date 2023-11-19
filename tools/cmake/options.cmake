option(FEJIX_PROTOCOL_X11 OFF)
option(FEJIX_PROTOCOL_WINAPI OFF)

if(FEJIX_PROTOCOL_WINAPI)
    add_compile_definitions("FJ_PROTOCOL_WINAPI")
endif()

if(FEJIX_PROTOCOL_X11)
    add_compile_definitions("FJ_PROTOCOL_X11")
endif()
