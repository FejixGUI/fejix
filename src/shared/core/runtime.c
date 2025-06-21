#include <src/shared/utils/memory.h>

#include <fejix/runtime.h>

#include <stdlib.h>
#include <string.h>


// Backends are in alphabetic order
void fj_wayland_init(void);
void fj_winapi_init(void);
void fj_x11_init(void);

// Backends are in alphabetic order
static char const *const backends[] = {
#ifdef FJ_COMPILE_OPT_ENABLE_WAYLAND_BACKEND
    "wayland",
#endif
#ifdef FJ_COMPILE_OPT_ENABLE_WINAPI_BACKEND
    "winapi",
#endif
#ifdef FJ_COMPILE_OPT_ENABLE_X11_BACKEND
    "x11",
#endif
    NULL,  // Avoid warnings about the array being empty
};

// Backends are in alphabetic order
static void (*init_funcs[])(void) = {
#ifdef FJ_COMPILE_OPT_ENABLE_WAYLAND_BACKEND
    fj_wayland_init,
#endif
#ifdef FJ_COMPILE_OPT_ENABLE_WINAPI_BACKEND
    fj_winapi_init,
#endif
#ifdef FJ_COMPILE_OPT_ENABLE_X11_BACKEND
    fj_x11_init,
#endif
    NULL,  // Avoid warnings about the array being empty
};

static uint32_t const backends_length = FJ_LEN(backends) - 1;  // Accounting for the NULL at the end

void fj_backend_get_list(char const *const **out_backends, uint32_t *out_backends_length)
{
    *out_backends = backends;
    *out_backends_length = backends_length;
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

#if defined(FJ_COMPILE_OPT_ENABLE_WAYLAND_BACKEND) || defined(FJ_COMPILE_OPT_ENABLE_X11_BACKEND)
    if (getenv("XDG_SESSION_TYPE") != NULL) {
        if (strcmp(getenv("XDG_SESSION_TYPE"), "wayland") == 0) {
            return "wayland";
        }

        if (strcmp(getenv("XDG_SESSION_TYPE"), "x11") == 0) {
            return "x11";
        }
    }
#endif

#if defined(FJ_COMPILE_OPT_ENABLE_WAYLAND_BACKEND)
    if (getenv("WAYLAND_DISPLAY") != NULL) {
        return "wayland";
    }
#endif

#if defined(FJ_COMPILE_OPT_ENABLE_X11_BACKEND)
    if (getenv("DISPLAY") != NULL) {
        return "x11";
    }
#endif

    return backends[0];  // This is NULL if there are no backends
}

enum fj_error fj_backend_select(char const *backend_name)
{
    for (uint32_t i = 0; i < backends_length; i++) {
        if (strcmp(backends[i], backend_name) == 0) {
            init_funcs[i]();
            return FJ_OK;
        }
    }

    return FJ_ERROR_UNAVAILABLE;
}
