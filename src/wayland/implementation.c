#include <fejix/interface/implementation.h>

#include <fejix/core/utils.h>


extern struct fj_client_iface const fj_wayland_client_iface;
extern struct fj_layer_iface const fj_wayland_layer_iface;

#ifdef FJ_OPT_FEATURE_SOFTER_CANVAS
extern struct fj_softer_canvas_iface const fj_wayland_softer_canvas_iface;
#endif

static
void const * ifaces[] = {
    [FJ_INTERFACE_CLIENT] = &fj_wayland_client_iface,
    [FJ_INTERFACE_LAYER] = &fj_wayland_layer_iface,

#ifdef FJ_OPT_FEATURE_SOFTER_CANVAS
    [FJ_INTERFACE_SOFTER_CANVAS] = &fj_wayland_softer_canvas_iface,
#endif
};


static
void const */*?*/ implementation_get(fj_interface_id_t id)
{
    if (id >= FJ_ARRAY_LEN(ifaces)) {
        return NULL;
    }

    if (ifaces[id] == NULL) {
        return NULL;
    }

    return ifaces[id];
}


struct fj_implementation_iface fj_wayland_implementation_iface = {
    .get = implementation_get,
    .id = FJ_IMPLEMENTATION_WAYLAND,
    .version = FJ_VERSION(0, 0, 0),
};
