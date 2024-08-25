#include <fejix/interface/implementation.h>

#include <fejix/core/utils.h>


extern struct fj_client_iface const fj_wayland_client_impl;
extern struct fj_layer_iface const fj_wayland_layer_impl;

static
void const * impls[] = {
    [FJ_INTERFACE_CLIENT] = &fj_wayland_client_impl,
    [FJ_INTERFACE_LAYER] = &fj_wayland_layer_impl,
};


static
void const */*?*/ implementation_iface_get(fj_interface_id_t id)
{
    if (id >= FJ_ARRAY_LEN(impls)) {
        return NULL;
    }

    if (impls[id] == NULL) {
        return NULL;
    }

    return impls[id];
}


struct fj_implementation_iface fj_wayland_implementation_impl = {
    .get = implementation_iface_get,
    .id = FJ_IMPLEMENTATION_WAYLAND,
    .version = FJ_VERSION(0, 0, 0),
};
