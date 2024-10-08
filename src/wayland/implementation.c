#include <fejix/interface/implementation.h>

#include <fejix/core/utils.h>


extern struct fj_client_iface const fj_client_iface;
extern struct fj_output_iface const fj_wayland_output_iface;
extern struct fj_softer_canvas_iface const fj_wayland_softer_canvas_iface;

static void const *interfaces[] = {
    [FJ_INTERFACE_CLIENT] = &fj_client_iface,
    [FJ_INTERFACE_OUTPUT] = &fj_wayland_output_iface,
#ifdef FJ_OPT_FEATURE_SOFTER_CANVAS
    [FJ_INTERFACE_SOFTER_CANVAS] = &fj_wayland_softer_canvas_iface,
#endif
};


static void const *get_interface(fj_interface_id_t id)
{
    if (id >= FJ_ARRAY_LEN(interfaces)) {
        return NULL;
    }

    if (interfaces[id] == NULL) {
        return NULL;
    }

    return interfaces[id];
}


struct fj_implementation_iface fj_wayland_implementation_iface = {
    .get_interface = get_interface,
    .id = FJ_IMPLEMENTATION_WAYLAND,
    .version = FJ_VERSION(0, 0, 0),
};
