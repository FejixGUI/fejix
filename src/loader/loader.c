#include <src/loader/loader.h>

#include <fejix/loader/loader.h>

#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>

#include <stdlib.h>
#include <string.h>


void fj_loader_load_functions(struct fj_library const *library)
{
    for (uint32_t i = 0; i < fj_loader_function_count; i++) {
        *fj_loader_function_pointers[i]
            = library->load_function(library->library_data, fj_loader_function_names[i]);
    }
}


typedef uint32_t fj_loader_implementation_id_t;

enum {
    FJ_LOADER_IMPLEMENTATION_WINAPI = 1,
    FJ_LOADER_IMPLEMENTATION_WAYLAND,
    FJ_LOADER_IMPLEMENTATION_X11,
};


static fj_loader_implementation_id_t guess_implementation(void)
{
#if defined(FJ_OPT_LOADER_HAS_WAYLAND) + defined(FJ_OPT_LOADER_HAS_X11) > 1
    {
        char const *env = getenv("XDG_SESSION_TYPE");

        if (env != NULL) {
            if (strcmp(env, "wayland") == 0) {
                return FJ_LOADER_IMPLEMENTATION_WAYLAND;
            } else if (strcmp(env, "x11") == 0) {
                return FJ_LOADER_IMPLEMENTATION_X11;
            }
        }

        env = getenv("WAYLAND_DISPLAY");

        if (env != NULL) {
            return FJ_LOADER_IMPLEMENTATION_WAYLAND;
        }

        env = getenv("DISPLAY");

        if (env != NULL) {
            return FJ_LOADER_IMPLEMENTATION_X11;
        }

        // XXX Default to X11?
        return FJ_LOADER_IMPLEMENTATION_X11;
    }
#else
#    if defined(FJ_OPT_WINAPI)
    return FJ_IMPLEMENTATION_WINAPI;
#    elif defined(FJ_OPT_LOADER_HAS_WAYLAND)
    return FJ_IMPLEMENTATION_WAYLAND;
#    elif defined(FJ_OPT_LOADER_HAS_X11)
    return FJ_IMPLEMENTATION_X11;
#    else
    return 10000;  // this result will not be used anyway
#    endif
#endif
}


char const *library_paths[] = {
    [0] = NULL,  // Prevents warnings about this array being empty
#ifdef FJ_OPT_LOADER_HAS_WINAPI
    [FJ_IMPLEMENTATION_WINAPI] = "fejix_winapi.dll",
#endif
#ifdef FJ_OPT_LOADER_HAS_WAYLAND
    [FJ_LOADER_IMPLEMENTATION_WAYLAND] = "fejix_wayland.so",
#endif
#ifdef FJ_OPT_LOADER_HAS_X11
    [FJ_LOADER_IMPLEMENTATION_X11] = "fejix_x11.so",
#endif
};


static struct fj_library library;


fj_err_t fj_loader_load(void)
{
    if (FJ_LEN(library_paths) == 1) {
        return FJ_ERR_CANNOT_LOAD_LIBRARY;
    }

    char const *library_path = library_paths[guess_implementation()];

    FJ_TRY (fj_library_load(&library, library_path)) {
        return fj_result;
    }

    fj_loader_load_functions(&library);

    return FJ_OK;
}
