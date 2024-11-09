#include <fejix/implementation.h>

#include <fejix/core/utils.h>


extern struct fj_client_funcs const fj_wayland_client_funcs;

static void const *interfaces[] = {
    [FJ_INTERFACE_CLIENT] = &fj_wayland_client_funcs,
};


static void const *get_interface_funcs(fj_interface_id_t id)
{
    if (id >= FJ_ARRAY_LEN(interfaces)) {
        return NULL;
    }

    if (interfaces[id] == NULL) {
        return NULL;
    }

    return interfaces[id];
}


struct fj_implementation fj_wayland_implementation = {
    .get_interface_funcs = get_interface_funcs,
    .id = FJ_IMPLEMENTATION_WAYLAND,
    .version = FJ_VERSION(0, 0, 0),
};
