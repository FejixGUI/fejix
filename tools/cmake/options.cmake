option(FEJIX_BUILD_TESTS OFF)

fejix_add_optional_definition(DEBUG)

fejix_add_optional_definition(PLATFORM_X11)
fejix_add_optional_definition(PLATFORM_WAYLAND)
fejix_add_optional_definition(PLATFORM_WINAPI)
fejix_add_optional_definition(PLATFORM_COCOA)

fejix_add_optional_definition(INTERFACE_WMANIP)
fejix_add_optional_definition(INTERFACE_WINPUT)
fejix_add_optional_definition(INTERFACE_SYSREDRAW)
fejix_add_optional_definition(INTERFACE_CLIPBOARD)

fejix_add_optional_definition(INTERFACE_PIXMAP)
fejix_add_optional_definition(INTERFACE_OPENGL)
