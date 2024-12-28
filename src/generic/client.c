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

static fj_responder_t *const implementation_responders[] = {
#ifdef FJ_OPT_WAYLAND
    &fj_wayland_client_responder,
#endif
#ifdef FJ_OPT_WINAPI
    &fj_winapi_client_responder,
#endif
#ifdef FJ_OPT_X11
    &fj_x11_client_responder,
#endif
};


static fj_client_implementation_id_t implementation_get_id(fj_responder_t responder)
{
    fj_client_implementation_id_t id;
    FJ_TRY (responder(NULL, FJ_CLIENT_GET_IMPLEMENTATION_ID, NULL, &id)) {
        return UINT32_MAX;
    }

    return id;
}


static char const *implementation_get_name(fj_responder_t responder)
{
    fj_client_implementation_id_t id = implementation_get_id(responder);

    if (id >= FJ_LEN(implementation_names) || implementation_names[id] == NULL) {
        return NULL;
    }

    return implementation_names[id];
}


fj_responder_t fj_client_get_builtin_implementation(fj_client_implementation_id_t id)
{
    for (uint32_t i = 0; i < FJ_LEN(implementation_responders); i++) {
        if (implementation_get_id(*implementation_responders[i]) == id) {
            return *implementation_responders[i];
        }
    }

    return NULL;
}


static fj_err_t find_implementation_with_name(
    fj_client_implementation_id_t *out_id,
    char const *name
)
{
    if (name == NULL) {
        return FJ_ERR_NOT_FOUND;
    }

    for (uint32_t i = 0; i < FJ_LEN(implementation_responders); i++) {
        if (fj_str_eq(implementation_get_name(*implementation_responders[i]), name)) {
            *out_id = implementation_get_id(*implementation_responders[i]);
            return FJ_OK;
        }
    }

    return FJ_ERR_NOT_FOUND;
}


fj_err_t fj_client_get_default_implementation_id(fj_client_implementation_id_t *out_id)
{
    if (FJ_LEN(implementation_responders) == 1) {
        *out_id = implementation_get_id(*implementation_responders[0]);
        return FJ_OK;
    }

    char const *var = getenv("FEJIX_PREFERRED_IMPLEMENTATION");

    if (find_implementation_with_name(out_id, var) == FJ_OK) {
        return FJ_OK;
    }

    var = getenv("XDG_SESSION_TYPE");

    if ((fj_str_eq(var, "wayland") || fj_str_eq(var, "x11"))
        && find_implementation_with_name(out_id, var) == FJ_OK)
    {
        return FJ_OK;
    }

    if (getenv("WAYLAND_DISPLAY") != NULL
        && find_implementation_with_name(out_id, "wayland") == FJ_OK)
    {
        return FJ_OK;
    }

    if (getenv("DISPLAY") != NULL && find_implementation_with_name(out_id, "x11") == FJ_OK) {
        return FJ_OK;
    }

    return FJ_ERR_CANNOT_SELECT_IMPLEMENTATION;
}
