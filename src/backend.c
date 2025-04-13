#include <fejix/backend.h>
#include <fejix/utils/memory.h>

#include <stdlib.h>
#include <string.h>


void fj_init_methods_x11(void);
void fj_init_methods_winapi(void);
void fj_init_methods_wayland(void);

// Backends are in alphabetic order
static char const *const backends[] = {
#ifdef FJ_BUILDING_WAYLAND
    "wayland",
#endif
#ifdef FJ_BUILDING_WINAPI
    "winapi",
#endif
#ifdef FJ_BUILDING_X11
    "x11",
#endif
    NULL,  // Avoid warnings about an empty array
};

static uint32_t const backends_length = FJ_LEN(backends) - 1;

// Backends are in alphabetic order
static void (*method_initialization_funcs[])(void) = {
#ifdef FJ_BUILDING_WAYLAND
    fj_init_methods_wayland,
#endif
#ifdef FJ_BUILDING_WINAPI
    fj_init_methods_winapi,
#endif
#ifdef FJ_BUILDING_X11
    fj_init_methods_x11,
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
    {
        char const *hint = getenv("FEJIX_BACKEND_HINT");

        if (hint != NULL) {
            for (uint32_t i = 0; i < backends_length; i++) {
                if (strcmp(backends[i], hint) == 0) {
                    return backends[i];
                }
            }
        }
    }

#if defined(FJ_BUILDING_WAYLAND) || defined(FJ_BUILDING_X11)
    if (getenv("XDG_SESSION_TYPE") != NULL) {
        if (strcmp(getenv("XDG_SESSION_TYPE"), "wayland") == 0) {
            return "wayland";
        }

        if (strcmp(getenv("XDG_SESSION_TYPE"), "x11") == 0) {
            return "x11";
        }
    }
#endif

#if defined(FJ_BUILDING_WAYLAND)
    if (getenv("WAYLAND_DISPLAY") != NULL) {
        return "wayland";
    }
#endif

#if defined(FJ_BUILDING_X11)
    if (getenv("DISPLAY") != NULL) {
        return "x11";
    }
#endif

    return backends[0];
}


#ifdef FJ_BUILDING_SINGLE_BACKEND
fj_err fj_backend_select(char const *backend_name)
{
    // All functions are already initialized statically in this case
    (void) backend_name;
    return FJ_OK;
}
#else
fj_err fj_backend_select(char const *backend_name)
{
    for (uint32_t i = 0; i < backends_length; i++) {
        if (strcmp(backends[i], backend_name) == 0) {
            method_initialization_funcs[i]();
        }
    }

    return FJ_ERR_UNAVAILABLE;
}
#endif
