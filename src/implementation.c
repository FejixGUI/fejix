#include <fejix/implementation.h>

#include <fejix/core/utils.h>

#include <stdlib.h>


extern struct fj_implementation const fj_winapi_implementation;
extern struct fj_implementation const fj_wayland_implementation;

static struct fj_implementation const *const implementations[] = {
#ifdef FJ_OPT_WINAPI
    [FJ_IMPLEMENTATION_WINAPI] = &fj_winapi_implementation,
#endif
#ifdef FJ_OPT_WAYLAND
    [FJ_IMPLEMENTATION_WAYLAND] = &fj_wayland_implementation,
#endif
    NULL,  // Keeps this array non-empty, avoids warnings.
};


static char const *const implementation_names[] = {
    [FJ_IMPLEMENTATION_ANDK] = "andk",       [FJ_IMPLEMENTATION_COCOA] = "cocoa",
    [FJ_IMPLEMENTATION_WAYLAND] = "wayland", [FJ_IMPLEMENTATION_WINAPI] = "winapi",
    [FJ_IMPLEMENTATION_X11] = "x11",
};


char const *fj_implementation_get_name(fj_implementation_id_t id)
{
    if (id >= FJ_LEN(implementation_names) || implementation_names[id] == NULL) {
        return NULL;
    }

    return implementation_names[id];
}


struct fj_implementation const *fj_implementation_get_builtin(fj_implementation_id_t id)
{
    if (id >= FJ_LEN(implementations) || implementations[id] == NULL) {
        return NULL;
    }

    return implementations[id];
}


static fj_err_t find_with_name(fj_implementation_id_t *out_id, char const *name)
{
    if (name == NULL) {
        return FJ_ERR_NOT_FOUND;
    }

    for (uint32_t i = 0; i < FJ_LEN(implementations); i++) {
        if (fj_str_eq(name, fj_implementation_get_name(implementations[i]->id))) {
            *out_id = implementations[i]->id;
            return FJ_OK;
        }
    }

    return FJ_ERR_NOT_FOUND;
}


fj_err_t fj_implementation_get_default_id(fj_implementation_id_t *out_id)
{
    if (FJ_LEN(implementations) == 1) {
        *out_id = implementations[0]->id;
        return FJ_OK;
    }

    char const *var = getenv("FEJIX_PREFERRED_IMPLEMENTATION");

    if (find_with_name(out_id, var) == FJ_OK) {
        return FJ_OK;
    }

    var = getenv("XDG_SESSION_TYPE");

    if ((fj_str_eq(var, "wayland") || fj_str_eq(var, "x11"))
        && find_with_name(out_id, var) == FJ_OK)
    {
        return FJ_OK;
    }

    if (getenv("WAYLAND_DISPLAY") != NULL && find_with_name(out_id, "wayland") == FJ_OK) {
        return FJ_OK;
    }

    if (getenv("DISPLAY") != NULL && find_with_name(out_id, "x11") == FJ_OK) {
        return FJ_OK;
    }

    return FJ_ERR_CANNOT_SELECT_IMPLEMENTATION;
}
