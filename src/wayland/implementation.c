#include <fejix/implementation.h>


extern struct fj_client_iface fj_wayland_client_impl;


struct fj_implementation fj_wayland_implementation = {
    .id = FJ_IMPLEMENTATION_WAYLAND,
    .version = FJ_VERSION(0, 0, 0),
    .client = &fj_wayland_client_impl,
};
