#include <src/loader/loader.h>

#include <fejix/loader/loader.h>

#include <fejix/core/utils.h>

#include <dlfcn.h>

#include <stdlib.h>


static void *library;


static fj_loader_function_t load_function(char const *function_name)
{
    return (fj_loader_function_t) (uintptr_t) dlsym(library, function_name);
}


static void load_functions(void)
{
    for (uint32_t i = 0; i < fj_loader_function_count; i++) {
        *fj_loader_function_pointers[i] = load_function(fj_loader_function_names[i]);
    }
}

static void unload_functions(void)
{
    for (uint32_t i = 0; i < fj_loader_function_count; i++) {
        *fj_loader_function_pointers[i] = NULL;
    }
}


static fj_err_t load_library(char const *library_path)
{
    library = dlopen(library_path, RTLD_LAZY);

    if (library == NULL) {
        return FJ_ERR_CANNOT_LOAD_LIBRARY;
    }

    load_functions();

    return FJ_OK;
}


static void unload_library(void)
{
    unload_functions();
    dlclose(library);
}


static char const *get_default_library_path(void)
{
    // TODO loader-posix default library path
    return "fejix_wayland.dll";
}


fj_err_t fj_loader_load_library(char const *library_path)
{
    fj_err_t result = load_library(library_path);

    return result;
}


void fj_loader_unload_library(void)
{
    unload_library();
}


// static fj_err_t get_builtin_implementation_id_by_name(
//     fj_implementation_id_t *out_id, char const *name)
// {
//     for (uint32_t i = 0; i < get_implementation_count(); i++) {
//         if (strcmp(name, fj_implementation_get_name(implementations[i]->id)) == 0) {
//             *out_id = implementations[i]->id;
//             return FJ_OK;
//         }
//     }

//     return FJ_ERR_NOT_FOUND;
// }


// static fj_err_t guess_the_only_implementation(fj_implementation_id_t *out_id)
// {
//     if (get_implementation_count() != 1) {
//         return FJ_ERR_NOT_FOUND;
//     }

//     *out_id = implementations[0]->id;
//     return FJ_OK;
// }


// static fj_err_t guess_fejix_preferred_implementation(fj_implementation_id_t *out_id)
// {
//     char const *preferred_name = getenv("FEJIX_PREFERRED_IMPLEMENTATION");

//     if (preferred_name == NULL) {
//         return FJ_ERR_NOT_FOUND;
//     }

//     FJ_TRY (get_builtin_implementation_id_by_name(out_id, preferred_name)) {
//         return fj_result;
//     }

//     return FJ_OK;
// }


// static fj_err_t guess_xdg_session_type(fj_implementation_id_t *out_id)
// {
//     char const *session_type = getenv("XDG_SESSION_TYPE");

//     if (session_type == NULL) {
//         return FJ_ERR_NOT_FOUND;
//     }

//     if (strcmp(session_type, "wayland") == 0) {
//         *out_id = FJ_IMPLEMENTATION_WAYLAND;
//     } else if (strcmp(session_type, "x11") == 0) {
//         *out_id = FJ_IMPLEMENTATION_X11;
//     } else {
//         return FJ_ERR_UNKNOWN;
//     }

//     if (fj_implementation_get_builtin(FJ_IMPLEMENTATION_WAYLAND) == NULL) {
//         return FJ_ERR_NOT_FOUND;
//     }

//     return FJ_OK;
// }


// static fj_err_t guess_wayland_display(fj_implementation_id_t *out_id)
// {
//     if (getenv("WAYLAND_DISPLAY") != NULL) {
//         return FJ_ERR_NOT_FOUND;
//     }

//     if (fj_implementation_get_builtin(FJ_IMPLEMENTATION_WAYLAND) == NULL) {
//         return FJ_ERR_NOT_FOUND;
//     }

//     *out_id = FJ_IMPLEMENTATION_WAYLAND;
//     return FJ_OK;
// }


// static fj_err_t guess_x11_display(fj_implementation_id_t *out_id)
// {
//     if (getenv("DISPLAY") != NULL) {
//         return FJ_ERR_NOT_FOUND;
//     }

//     if (fj_implementation_get_builtin(FJ_IMPLEMENTATION_X11) == NULL) {
//         return FJ_ERR_NOT_FOUND;
//     }

//     *out_id = FJ_IMPLEMENTATION_X11;
//     return FJ_OK;
// }


// static fj_err_t guess_display(fj_implementation_id_t *out_id)
// {
//     if (guess_wayland_display(out_id) == FJ_OK) {
//         return FJ_OK;
//     }

//     if (guess_x11_display(out_id) == FJ_OK) {
//         return FJ_OK;
//     }

//     return FJ_ERR_NOT_FOUND;
// }


// fj_err_t fj_implementation_get_default_id(fj_implementation_id_t *out_id)
// {
//     if (guess_the_only_implementation(out_id) == FJ_OK) {
//         return FJ_OK;
//     }

//     if (guess_fejix_preferred_implementation(out_id) == FJ_OK) {
//         return FJ_OK;
//     }

//     if (guess_xdg_session_type(out_id) == FJ_OK) {
//         return FJ_OK;
//     }

//     if (guess_display(out_id) == FJ_OK) {
//         return FJ_OK;
//     }

//     return FJ_ERR_CANNOT_SELECT_IMPLEMENTATION;
// }
