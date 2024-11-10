#include <fejix/implementation.h>

#include <fejix/core/utils.h>


extern struct fj_client_funcs fj_winapi_client_funcs;
extern struct fj_client_wait_timeout_funcs fj_winapi_client_wait_timeout_funcs;
extern struct fj_window_funcs fj_winapi_window_funcs;

static void const *interface_funcs[] = {
    [FJ_INTERFACE_CLIENT] = &fj_winapi_client_funcs,
    [FJ_INTERFACE_SLEEP_TIMER] = &fj_winapi_client_wait_timeout_funcs,
    [FJ_INTERFACE_WINDOW] = &fj_winapi_window_funcs,
};


static void const *get_interface_funcs(fj_interface_id_t id)
{
    if (id >= FJ_ARRAY_LEN(interface_funcs) || interface_funcs[id] == NULL) {
        return NULL;
    }

    return interface_funcs[id];
}


struct fj_implementation const fj_winapi_implementation = {
    .id = FJ_IMPLEMENTATION_WINAPI,
    .version = FJ_VERSION(0, 0, 1),
    .get_interface_funcs = get_interface_funcs,
};
