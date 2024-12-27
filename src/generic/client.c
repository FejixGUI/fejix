#include <fejix/interface/client.h>

#include <fejix/core/utils.h>

#include <stdlib.h>


extern fj_responder_t fj_andk_client_responder;
extern fj_responder_t fj_cocoa_client_responder;
extern fj_responder_t fj_wayland_client_responder;
extern fj_responder_t fj_winapi_client_responder;
extern fj_responder_t fj_x11_client_responder;

static char const *const implementation_names[] = {
    [FJ_CLIENT_IMPLEMENTATION_ANDK] = "andk",       [FJ_CLIENT_IMPLEMENTATION_COCOA] = "cocoa",
    [FJ_CLIENT_IMPLEMENTATION_WAYLAND] = "wayland", [FJ_CLIENT_IMPLEMENTATION_WINAPI] = "winapi",
    [FJ_CLIENT_IMPLEMENTATION_X11] = "x11",
};


fj_responder_t fj_client_get_builtin_implementation(fj_client_implementation_id_t id)
{
    switch (id) {
#ifdef FJ_OPT_WAYLAND
        case FJ_CLIENT_IMPLEMENTATION_WAYLAND:
            return fj_wayland_client_responder;
#endif
#ifdef FJ_OPT_WINAPI
        case FJ_CLIENT_IMPLEMENTATION_WINAPI:
            return fj_winapi_client_responder;
#endif
#ifdef FJ_OPT_X11
        case FJ_CLIENT_IMPLEMENTATION_X11:
            return fj_x11_client_responder;
#endif
    }

    return NULL;
}


static fj_err_t get_default_implementation_fejix(fj_client_implementation_id_t *out_id)
{
    char const *implementation_name = getenv("FEJIX_IMPLEMENTATION");

    if (implementation_name == NULL) {
        return FJ_ERR_NOT_FOUND;
    }

    for (uint32_t i = 0; i < FJ_LEN(implementation_names); i++) {
        if (fj_str_eq(implementation_names[i], implementation_name)) {
            *out_id = i;
            return FJ_OK;
        }
    }

    return FJ_ERR_NOT_FOUND;
}


static fj_err_t get_default_implementation_xdg(fj_client_implementation_id_t *out_id)
{
    if (fj_str_eq(getenv("XDG_SESSION_TYPE"), "wayland")) {
        *out_id = FJ_CLIENT_IMPLEMENTATION_WAYLAND;
        return FJ_OK;
    }

    if (fj_str_eq(getenv("XDG_SESSION_TYPE"), "x11")) {
        *out_id = FJ_CLIENT_IMPLEMENTATION_X11;
        return FJ_OK;
    }

    if (getenv("WAYLAND_DISPLAY") != NULL) {
        *out_id = FJ_CLIENT_IMPLEMENTATION_WAYLAND;
        return FJ_OK;
    }

    if (getenv("DISPLAY") != NULL) {
        *out_id = FJ_CLIENT_IMPLEMENTATION_X11;
        return FJ_OK;
    }

    return FJ_ERR_NOT_FOUND;
}


fj_err_t fj_client_get_default_implementation_id(fj_client_implementation_id_t *out_id)
{
    fj_client_implementation_id_t id;

    if (get_default_implementation_fejix(&id) != FJ_OK
        && get_default_implementation_xdg(&id) != FJ_OK)
    {
        return FJ_ERR_CANNOT_SELECT_IMPLEMENTATION;
    }

    if (fj_client_get_builtin_implementation(id) == NULL) {
        return FJ_ERR_CANNOT_SELECT_IMPLEMENTATION;
    }

    *out_id = id;
    return FJ_OK;
}
