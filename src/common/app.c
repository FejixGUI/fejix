#include <fejix/app.h>

#include <fejix/core/utils.h>

#include <stdlib.h>


extern struct fj_app_funcs const fj_winapi_app_funcs;
extern struct fj_app_funcs const fj_wayland_app_funcs;

static struct fj_app_funcs const *const implementations[] = {
#ifdef FJ_OPT_WINAPI
    [FJ_APP_IMPLEMENTATION_WINAPI] = &fj_winapi_app_funcs,
#endif
#ifdef FJ_OPT_WAYLAND
    [FJ_APP_IMPLEMENTATION_WAYLAND] = &fj_wayland_app_funcs,
#endif
    NULL,  // Keeps this array non-empty, avoids warnings.
};


static char const *const implementation_names[] = {
    [FJ_APP_IMPLEMENTATION_ANDK] = "andk",       [FJ_APP_IMPLEMENTATION_COCOA] = "cocoa",
    [FJ_APP_IMPLEMENTATION_WAYLAND] = "wayland", [FJ_APP_IMPLEMENTATION_WINAPI] = "winapi",
    [FJ_APP_IMPLEMENTATION_X11] = "x11",
};


char const *fj_app_get_implementation_name(fj_app_implementation_id_t id)
{
    if (id >= FJ_LEN(implementation_names) || implementation_names[id] == NULL) {
        return NULL;
    }

    return implementation_names[id];
}


struct fj_app_funcs const *fj_app_get_builtin_implementation(fj_app_implementation_id_t id)
{
    if (id >= FJ_LEN(implementations) || implementations[id] == NULL) {
        return NULL;
    }

    return implementations[id];
}


static fj_err_t find_with_name(fj_app_implementation_id_t *out_id, char const *name)
{
    if (name == NULL) {
        return FJ_ERR_NOT_FOUND;
    }

    for (uint32_t i = 0; i < FJ_LEN(implementations); i++) {
        fj_app_implementation_id_t id = implementations[i]->get_implementation_id();
        if (fj_str_eq(name, fj_app_get_implementation_name(id))) {
            *out_id = id;
            return FJ_OK;
        }
    }

    return FJ_ERR_NOT_FOUND;
}


static inline bool has_only_one_implementation(void)
{
    return FJ_LEN(implementations) == 1;
}


fj_err_t fj_app_get_default_implementation_id(fj_app_implementation_id_t *out_id)
{
    if (has_only_one_implementation()) {
        *out_id = implementations[0]->get_implementation_id();
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
