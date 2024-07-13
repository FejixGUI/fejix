#include <src/implementation/wayland/interface/client/wayland.h>

#include <fejix/core/error.h>


static
void wayland_handle_registry_add(
    void * data,
    struct wl_registry * registry,
    uint32_t interface_id,
    const char * interface_name,
    uint32_t interface_version
)
{

}


static
void wayland_handle_registry_remove(
    void * data,
    struct wl_registry * registry,
    uint32_t interface_id
)
{
}


static
struct wl_registry_listener const wayland_registry_listener = {
    .global = wayland_handle_registry_add,
    .global_remove = wayland_handle_registry_remove,
};



fj_err_t fj_wayland_init(struct fj_wayland_client_data * client)
{
    client->display = wl_display_connect(NULL);

    if (client->display == NULL) {
        return FJ_ERR_SHELL_CONNECTION_ERROR;
    }

    client->registry = wl_display_get_registry(client->display);

    wl_registry_add_listener(
        client->registry,
        &wayland_registry_listener,
        client
    );

    wl_display_roundtrip(client->display);

    wl_display_dispatch(client->display);

    return FJ_OK;
}


fj_err_t fj_wayland_deinit(struct fj_wayland_client_data * client)
{
    wl_display_disconnect(client->display);

    return FJ_OK;
}
