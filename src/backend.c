#include <fejix/backend.h>
#include <fejix/utils/memory.h>

#include <stdlib.h>
#include <string.h>


#ifdef FJ_BUILDING_X11
void fj_x11_init_methods(void);
#endif
#ifdef FJ_BUILDING_WINAPI
void fj_winapi_init_methods(void);
#endif
#ifdef FJ_BUILDING_WAYLAND
void fj_wayland_init_methods(void);
#endif

static char const *const backends[] = {
#ifdef FJ_BUILDING_X11
    "X11",
#endif
#ifdef FJ_BUILDING_WINAPI
    "WindowsAPI",
#endif
#ifdef FJ_BUILDING_WAYLAND
    "Wayland",
#endif
    NULL,  // Avoid warnings about empty array
};

static void (*method_initialization_funcs[])(void) = {
#ifdef FJ_BUILDING_X11
    fj_x11_init_methods,
#endif
#ifdef FJ_BUILDING_WINAPI
    fj_winapi_init_methods,
#endif
#ifdef FJ_BUILDING_WAYLAND
    fj_wayland_init_methods,
#endif
    NULL,
};


void fj_backend_get_list(char const *const **out_backends, uint32_t *out_backends_length)
{
    *out_backends = backends;
    *out_backends_length = FJ_LEN(backends) - 1;
}


char const *fj_backend_get_default(void)
{
    if (getenv("FEJIX_BACKEND_HINT") != NULL) {
        char const *hint = getenv("FEJIX_BACKEND_HINT");
        for (uint32_t i = 0; i < FJ_LEN(backends) - 1; i++) {
            if (strcmp(backends[i], hint) == 0) {
                return backends[i];
            }
        }
    }

#if defined(FJ_BUILDING_WAYLAND)
    if (getenv("XDG_SESSION_TYPE") != NULL) {
        if (strcmp(getenv("XDG_SESSION_TYPE"), "wayland") == 0) {
            return "Wayland";
        }
    } else if (getenv("WAYLAND_DISPLAY") != NULL) {
        return "Wayland";
    }
#endif

#if defined(FJ_BUILDING_X11)
    if (getenv("XDG_SESSION_TYPE") != NULL) {
        if (strcmp(getenv("XDG_SESSION_TYPE"), "x11") == 0) {
            return "X11";
        }
    } else if (getenv("DISPLAY") != NULL) {
        return "X11";
    }
#endif

    return backends[0];
}


fj_err fj_backend_select(char const *backend_name)
{
    for (uint32_t i = 0; i < FJ_LEN(backends) - 1; i++) {
        if (strcmp(backends[i], backend_name) == 0) {
            method_initialization_funcs[i]();
        }
    }

    return FJ_ERR_UNAVAILABLE;
}
