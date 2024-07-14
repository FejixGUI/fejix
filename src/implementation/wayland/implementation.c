#include <fejix/implementation.h>


extern struct fj_client fj_wayland_client;


struct fj_implementation fj_wayland_implementation = {
    .implementation_id = FJ_IMPLEMENTATION_WAYLAND,
    .client = &fj_wayland_client,
};
