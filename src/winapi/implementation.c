#include <fejix/interface/implementation.h>

#include <fejix/core/utils.h>


extern struct fj_client_iface fj_winapi_client_iface;
extern struct fj_scheduler_iface fj_winapi_scheduler_iface;

static void const *interfaces[] = {
    [FJ_INTERFACE_CLIENT] = &fj_winapi_client_iface,
    [FJ_INTERFACE_SCHEDULER] = &fj_winapi_scheduler_iface,
};


static void const *get_interface(fj_interface_id_t id)
{
    if (id >= FJ_ARRAY_LEN(interfaces) || interfaces[id] == NULL) {
        return NULL;
    }

    return interfaces[id];
}


struct fj_implementation_iface const fj_winapi_implementation_iface = {
    .get_interface = get_interface,

    .id = FJ_IMPLEMENTATION_WINAPI,
    .version = FJ_VERSION(0, 0, 1),
};
