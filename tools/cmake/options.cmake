set(
    FEJIX_OPTIONS

    # Protocols
    "FJ_OPT_WINAPI"
    "FJ_OPT_X11"
    "FJ_OPT_WAYLAND"
    "FJ_OPT_COCOA"

    # Graphics APIs
    "FJ_OPT_PIXMAP"
    "FJ_OPT_OPENGL"
    "FJ_OPT_VULKAN"

    # Generic features
    "FJ_OPT_CLIPBOARD"
    "FJ_OPT_ICONTRAY"
)

foreach(option ${FEJIX_OPTIONS})
    option("${option}" OFF)
endforeach()

if(
    NOT FJ_OPT_WINAPI
    AND NOT FJ_OPT_X11
    AND NOT FJ_OPT_WAYLAND
    AND NOT FJ_OPT_COCOA
)
    message(
        FATAL_ERROR
        "***** You must enable at least one protocol (FJ_OPT_XXX=ON)"
    )
endif()

foreach(option ${FEJIX_OPTIONS})
    if(${option})
        add_compile_definitions("${option}")
    endif()
endforeach()
