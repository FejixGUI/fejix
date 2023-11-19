if(FEJIX_PROTOCOL_X11)

    target_link_libraries(fejix "X11" "X11-xcb" "xcb")

    target_link_libraries(fejix "xcb-sync")

    if(FEJIX_FEATURE_WINMANAGER)
        target_link_libraries(fejix "xcb-icccm")
    endif()

endif()

if(FEJIX_PROTOCOL_WINAPI)
    target_compile_definitions(fejix PRIVATE "UNICODE" "_UNICODE")
endif()
