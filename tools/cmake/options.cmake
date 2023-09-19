# List of planned options:
# PLATFORM_X11
# PLATFORM_WAYLAND
# PLATFORM_WINAPI
# PLATFORM_COCOA
# PLATFORM_UIKIT
# PLATFORM_ANDROID
# FEATURE_UNIXPOLLER
# FEATURE_SHELL
# FEATURE_WM
# FEATURE_INPUT
# FEATURE_INPUTMETHOD
# FEATURE_CLIPBOARD
# FEATURE_GRAPHICS
# FEATURE_RASTER
# FEATURE_OPENGL
# FEATURE_VULKAN
# FEATURE_DIRECTX
# FEATURE_METAL
# FEATURE_SYSREDRAW
# FEATURE_SYSINFO
# FEATURE_SYSTRAY

set(fejix_options "")

macro(declare_option option)
    option(FEJIX_${option} OFF)
    list(APPEND fejix_options "${option}")
endmacro()

function(define_macros_for_enabled_options)
    foreach(option ${fejix_options})
        if(FEJIX_${option})
            add_compile_definitions("FJ_${option}")
        endif()
    endforeach()
endfunction()


declare_option(PLATFORM_X11)
declare_option(PLATFORM_WINAPI)
declare_option(FEATURE_UNIXPOLLER)

if(FEJIX_PLATFORM_X11)
    set(FEJIX_FEATURE_UNIXPOLLER ON)
endif()

define_macros_for_enabled_options()
