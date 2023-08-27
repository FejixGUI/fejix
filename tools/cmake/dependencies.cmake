if(FEJIX_PLATFORM_X11)
    
    target_link_libraries(fejix "X11" "X11-xcb" "xcb")

    if(FEJIX_FEATURE_SYSREDRAW)
        target_link_libraries(fejix "xcb-sync")
    endif()
    
    if(FEJIX_FEATURE_WINMANAGER)
        target_link_libraries(fejix "xcb-icccm")
    endif()

endif()
