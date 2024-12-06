#include <fejix/implementation.h>

#include <fejix/core/utils.h>


extern struct fj_client_interface fj_winapi_client_interface;
extern struct fj_client_wait_timeout_interface fj_winapi_client_wait_timeout_interface;
extern struct fj_window_interface fj_winapi_window_interface;

static void const *interfaces[] = {
    [FJ_INTERFACE_CLIENT] = &fj_winapi_client_interface,
    [FJ_INTERFACE_CLIENT_WAIT_TIMEOUT] = &fj_winapi_client_wait_timeout_interface,
    [FJ_INTERFACE_WINDOW] = &fj_winapi_window_interface,
};


static void const *get_interface(fj_interface_id_t id)
{
    if (id >= FJ_ARRAY_LEN(interfaces) || interfaces[id] == NULL) {
        return NULL;
    }

    return interfaces[id];
}


struct fj_implementation const fj_winapi_implementation = {
    .id = FJ_IMPLEMENTATION_WINAPI,
    .version = FJ_VERSION(0, 0, 1),
    .get_interface = get_interface,
};
