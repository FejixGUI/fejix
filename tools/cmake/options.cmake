set(
    FEJIX_OPTIONS

    # Buses
    "FJ_OPT_ANDK"
    "FJ_OPT_COCOA"
    "FJ_OPT_NOOP"
    "FJ_OPT_X11"
    "FJ_OPT_WAYLAND"
    "FJ_OPT_WINAPI"

    # Graphics APIs
    "FJ_OPT_PIXMAP"
    "FJ_OPT_OPENGL"
    "FJ_OPT_VULKAN"

    # Generic features
    "FJ_OPT_CLIPBOARD"
    "FJ_OPT_ICONTRAY"

    # Other components
    "FJ_OPT_FDPOLL"
)

foreach(option ${FEJIX_OPTIONS})
    option("${option}" OFF)
endforeach()



if(
        NOT FJ_OPT_ANDK
    AND NOT FJ_OPT_COCOA
    AND NOT FJ_OPT_X11
    AND NOT FJ_OPT_WAYLAND
    AND NOT FJ_OPT_WINAPI

    AND NOT FJ_OPT_NOOP
)
    message(WARNING "<<<FEJIX>>> No bus specified, selecting a noop bus.")
    set(FJ_OPT_NOOP ON)
endif()

if(FJ_OPT_NOOP)
    message(WARNING "<<<FEJIX>>> Noop bus is enabled!")
endif()

if(FJ_OPT_WAYLAND OR FJ_OPT_X11)
    set(FJ_OPT_FDPOLL ON)
endif()



foreach(option ${FEJIX_OPTIONS})
    if(${option})
        add_compile_definitions("${option}")
    endif()
endforeach()
