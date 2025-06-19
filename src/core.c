#include <fejix/core.h>
#include <fejix/utils/memory.h>

#include <stdlib.h>
#include <string.h>


/** Each element is a string or NULL if unimplemented. */
static char const *status_descriptions[FJ_STATUS_MAX] = {
    [FJ_STATUS_OK] = "success",
    [FJ_STATUS_OUT_OF_MEMORY] = "out of memory",
    [FJ_STATUS_UNIMPLEMENTED] = "unimplemented",
    [FJ_STATUS_OPERATION_FAILED] = "operation failed",
    [FJ_STATUS_IO_FAILED] = "input/output failed",
    [FJ_STATUS_UNAVAILABLE] = "feature unavailable on the system",
    [FJ_STATUS_ACCESS_DENIED] = "access denied",
    [FJ_STATUS_CONCURRENT_ACCESS] = "concurrent access not permitted",
    [FJ_STATUS_INVALID_USAGE] = "invalid usage (bug)",
    [FJ_STATUS_INVALID_OPERATION] = "invalid operation",
    [FJ_STATUS_INVALID_ENCODING] = "invalid text encoding",
};


char const *fj_status_get_description(enum fj_status s)
{
    if (s >= FJ_STATUS_MAX || status_descriptions[s] == NULL) {
        return "unnamed error (did someone forget to write a description?)";
    }

    return status_descriptions[s];
}


void fj_init_methods_x11(void);
void fj_init_methods_winapi(void);
void fj_init_methods_wayland(void);

// Backends are in alphabetic order
static char const *const backends[] = {
#ifdef FJ_COMPILE_OPT_WAYLAND
    "wayland",
#endif
#ifdef FJ_COMPILE_OPT_WINAPI
    "winapi",
#endif
#ifdef FJ_COMPILE_OPT_X11
    "x11",
#endif
    NULL,  // Avoid warnings about the array being empty
};

// Backends are in alphabetic order
static void (*method_initialization_funcs[])(void) = {
#ifdef FJ_COMPILE_OPT_WAYLAND
    fj_init_methods_wayland,
#endif
#ifdef FJ_COMPILE_OPT_WINAPI
    fj_init_methods_winapi,
#endif
#ifdef FJ_COMPILE_OPT_X11
    fj_init_methods_x11,
#endif
    NULL,  // Avoid warnings about the array being empty
};

static uint32_t const backends_length = FJ_LEN(backends) - 1;

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

#if defined(FJ_COMPILE_OPT_WAYLAND) || defined(FJ_COMPILE_OPT_X11)
    if (getenv("XDG_SESSION_TYPE") != NULL) {
        if (strcmp(getenv("XDG_SESSION_TYPE"), "wayland") == 0) {
            return "wayland";
        }

        if (strcmp(getenv("XDG_SESSION_TYPE"), "x11") == 0) {
            return "x11";
        }
    }
#endif

#if defined(FJ_COMPILE_OPT_WAYLAND)
    if (getenv("WAYLAND_DISPLAY") != NULL) {
        return "wayland";
    }
#endif

#if defined(FJ_COMPILE_OPT_X11)
    if (getenv("DISPLAY") != NULL) {
        return "x11";
    }
#endif

    return backends[0];  // This is NULL if there are no backends
}

enum fj_status fj_backend_select(char const *backend_name)
{
    for (uint32_t i = 0; i < backends_length; i++) {
        if (strcmp(backends[i], backend_name) == 0) {
            method_initialization_funcs[i]();
            return FJ_STATUS_OK;
        }
    }

    return FJ_STATUS_UNAVAILABLE;
}
