if(FJ_OPT_X11)

    target_link_libraries(fejix "X11" "X11-xcb" "xcb" "xcb-sync" "xcb-icccm")

endif()

if(FJ_OPT_WINAPI)

    target_compile_definitions(fejix PRIVATE "UNICODE" "_UNICODE")

endif()
