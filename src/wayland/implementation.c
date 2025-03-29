#include <fejix/implementation.h>

#include <fejix/core/utils.h>


extern struct fj_client_functions const fj_wayland_client_functions;

static void const *interfaces[] = {
    [FJ_INTERFACE_CLIENT] = &fj_wayland_client_functions,
};


static void const *get_interface_functions(fj_interface_id id)
{
    if (id >= FJ_LEN(interfaces)) {
        return NULL;
    }

    if (interfaces[id] == NULL) {
        return NULL;
    }

    return interfaces[id];
}


struct fj_implementation fj_wayland_implementation = {
    .id = FJ_IMPLEMENTATION_WAYLAND,
    .version = FJ_VERSION(0, 0, 0),
    .get_interface_functions = get_interface_functions,
};
