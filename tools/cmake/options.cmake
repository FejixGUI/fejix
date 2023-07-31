macro(fejix_define_platform platform)

    option(FEJIX_PLATFORM_${platform} OFF)

    if(FEJIX_PLAFORM_${platform})
        target_compile_definitions(fejix PUBLIC "FJ_PLATFORM_${platform}")
    endif()
    
endmacro()


macro(fejix_define_interface interface)

    option(FEJIX_INTERFACE_${platform} OFF)

    if(FEJIX_INTERFACE_${platform})
        target_compile_definitions(fejix PUBLIC "FJ_INTERFACE_${platform}")
    endif()
    
endmacro()


fejix_define_platform(X11)
fejix_define_platform(WAYLAND)
fejix_define_platform(WINAPI)
fejix_define_platform(COCOA)

fejix_define_interface(WMANIP)
fejix_define_interface(WINPUT)
fejix_define_interface(SYSREDRAW)
fejix_define_interface(CLIPBOARD)

fejix_define_interface(PIXMAP)
fejix_define_interface(OPENGL)

option(FEJIX_BUILD_TESTS OFF)
