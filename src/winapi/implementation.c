#include <fejix/interface/implementation.h>


#define CASE(IFACE_UPPERCASE, IFACE_LOWERCASE) \
    case FJ_INTERFACE_##IFACE_UPPERCASE: { \
        extern struct fj_##IFACE_LOWERCASE##_iface fj_wayland_##IFACE_LOWERCASE##_impl; \
        return &fj_wayland_##IFACE_LOWERCASE##_impl; \
    }

static
void const * get(fj_interface_id_t id)
{
    switch (id) {
        default: return NULL;
        CASE(CLIENT, client)
        // CASE(LAYER, layer)
    }
}

#undef CASE


struct fj_implementation_iface const fj_winapi_implementation_impl = {
    .get = get,

    .id = FJ_IMPLEMENTATION_WINAPI,
    .version = FJ_VERSION(0, 0, 1),
};
