#include <fejix/interface/implementation.h>

#include <fejix/core/utils.h>

#include <loader.h>

#include <stdlib.h>
#include <string.h>

int a(void)
{
    return 1;
}
// extern struct fj_implementation const fj_winapi_implementation;
// extern struct fj_implementation const fj_wayland_implementation;

// static struct fj_implementation const *const implementations[] = {
// #ifdef FJ_OPT_WAYLAND
//     [FJ_IMPLEMENTATION_WAYLAND] = &fj_wayland_implementation,
// #endif
// #ifdef FJ_OPT_WINAPI
//     [FJ_IMPLEMENTATION_WINAPI] = &fj_winapi_implementation,
// #endif
//     NULL,  // Keeps this array non-empty, avoids warnings.
// };


// static char const *const implementation_names[] = {
//     [FJ_IMPLEMENTATION_X11] = "x11",       [FJ_IMPLEMENTATION_WAYLAND] = "wayland",
//     [FJ_IMPLEMENTATION_ANDK] = "andk",     [FJ_IMPLEMENTATION_COCOA] = "cocoa",
//     [FJ_IMPLEMENTATION_WINAPI] = "winapi",
// };


// static uint32_t get_implementation_count(void)
// {
//     return FJ_LEN(implementations) - 1;  // accounting for the ending NULL
// }


// char const *fj_implementation_get_name(fj_implementation_id_t id)
// {
//     if (id >= FJ_LEN(implementation_names) || implementation_names[id] == NULL) {
//         return NULL;
//     }

//     return implementation_names[id];
// }


// struct fj_implementation const *fj_implementation_get_builtin(fj_implementation_id_t id)
// {
//     if (id >= FJ_LEN(implementations) || implementations[id] == NULL) {
//         return NULL;
//     }

//     return implementations[id];
// }


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
