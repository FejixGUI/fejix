if(FEJIX_PLATFORM_X11)
    
    target_link_libraries(fejix "X11" "X11-xcb" "xcb")

    if(FEJIX_INTERFACE_SYSREDRAW)
        target_link_libraries(fejix "xcb-sync")
    endif()
    
    if(FEJIX_INTERFACE_WMANIP)
        target_link_libraries(fejix "xcb-icccm")
    endif()

endif()
