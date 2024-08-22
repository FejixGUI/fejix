#include <src/wayland/client/client.h>

#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>

#include <wayland-client-core.h>
#include <wayland-client-protocol.h>


static
void handle_registry_add(
    void * client_,
    struct wl_registry * _registry,
    uint32_t object_id,
    char const * interface_name,
    uint32_t interface_version
)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_wayland_client *)
    FJ_ARG_UNUSED(registry)

    if (fj_streq(interface_name, wl_compositor_interface.name)) {
        client->compositor_id = object_id;
        client->compositor_version = interface_version;
    }
}


static
void handle_registry_remove(void * _data, struct wl_registry * _registry, uint32_t _object_id)
{
    FJ_ARG_UNUSED(data)
    FJ_ARG_UNUSED(registry)
    FJ_ARG_UNUSED(object_id)

    // TODO Handle wayland.registry.remove
}


static
struct wl_registry_listener const registry_listener = {
    .global = handle_registry_add,
    .global_remove = handle_registry_remove,
};


static
fj_err_t wayland_dispatch_events(struct fj_wayland_client * client)
{
    client->callback_error = FJ_OK;

    int dispatch_result = wl_display_dispatch_pending(client->display);

    fj_err_t callback_error = client->callback_error;
    client->callback_error = FJ_OK;

    if (dispatch_result == -1) {
        return FJ_ERR_MESSAGE_READ_ERROR;
    }

    return callback_error;
}


static
fj_err_t wayland_wait_and_dispatch_events(struct fj_wayland_client * client)
{
    client->callback_error = FJ_OK;

    int dispatch_result = wl_display_dispatch(client->display);

    fj_err_t callback_error = client->callback_error;
    client->callback_error = FJ_OK;

    if (dispatch_result == -1) {
        return FJ_ERR_MESSAGE_READ_ERROR;
    }

    if (callback_error != FJ_OK) {
        return callback_error;
    }

    int roundtrip_result = wl_display_roundtrip(client->display);

    if (roundtrip_result == -1) {
        return FJ_ERR_MESSAGE_SEND_ERROR;
    }

    return FJ_OK;
}


static
fj_err_t wayland_handle_poll_event(
    void * client_,
    fj_unixpoller_fd_t _file_descriptor,
    fj_unixpoller_event_mask_t _event_mask
)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_wayland_client *)
    FJ_ARG_UNUSED(file_descriptor)
    FJ_ARG_UNUSED(event_mask)

    FJ_INIT_TRY

    int result = wl_display_read_events(client->display);

    if (result == -1) {
        return FJ_ERR_MESSAGE_READ_ERROR;
    }

    FJ_TRY(wayland_dispatch_events(client)) {
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
fj_err_t wayland_init(struct fj_wayland_client * client)
{
    FJ_INIT_TRY

    client->display = wl_display_connect(NULL);

    if (client->display == NULL) {
        return FJ_ERR_SHELL_CONNECTION_ERROR;
    }

    FJ_TRY(fj_unixpoller_add(
        &client->unixpoller,
        wl_display_get_fd(client->display),
        POLLIN,
        wayland_handle_poll_event
    )) {
        wl_display_disconnect(client->display);
        return FJ_RESULT;
    }

    client->registry = wl_display_get_registry(client->display);

    wl_registry_add_listener(client->registry, &registry_listener, client);

    FJ_TRY(wayland_wait_and_dispatch_events(client)) {
        wl_display_disconnect(client->display);
        fj_unixpoller_deinit(&client->unixpoller);
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
fj_err_t wayland_deinit(struct fj_wayland_client * client)
{
    wl_display_disconnect(client->display);

    return FJ_OK;
}


static
fj_err_t wayland_prepare_poll(struct fj_wayland_client * client)
{
    FJ_INIT_TRY

    while (wl_display_prepare_read(client->display) != 0) {
        FJ_TRY(wayland_dispatch_events(client)) {
            return FJ_RESULT;
        }
    }

    int result = wl_display_flush(client->display);

    if (result == -1) {
        // XXX we do not support incremental display flushing for now
        return FJ_ERR_MESSAGE_SEND_ERROR;
    }

    return FJ_OK;
}


static
fj_err_t client_init_run(struct fj_wayland_client * client)
{
    return client->callbacks->idle(client->data);
}


static
fj_err_t client_prepare_poll(struct fj_wayland_client * client)
{
    return wayland_prepare_poll(client);
}


static
fj_err_t client_end_poll(struct fj_wayland_client * client)
{
    return client->callbacks->idle(client->data);
}


static
void client_cancel_poll(struct fj_wayland_client * client)
{
    wl_display_cancel_read(client->display);
}


static
fj_err_t client_init(struct fj_wayland_client * client, struct fj_client_info const * info)
{
    FJ_INIT_TRY

    FJ_TRY(fj_strdup(info->name, &client->name)) {
        return FJ_RESULT;
    }

    FJ_TRY(fj_unixpoller_init(&client->unixpoller, (void *) client)) {
        FJ_FREE(&client->name);
        return FJ_RESULT;
    }

    FJ_TRY(wayland_init(client)) {
        FJ_FREE(&client->name);
        fj_unixpoller_deinit(&client->unixpoller);
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
void client_deinit(struct fj_wayland_client * client)
{
    wayland_deinit(client);
    fj_unixpoller_deinit(&client->unixpoller);
    FJ_FREE(&client->name);
}


static
fj_err_t client_create(
    fj_client_t */*? out*/ * client_,
    struct fj_client_callbacks const * callbacks,
    void * callback_data,
    struct fj_client_info const * info
)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_wayland_client */*? out*/ *)

    FJ_INIT_TRY

    FJ_TRY(FJ_ALLOC_ZEROED(client)) {
        return FJ_RESULT;
    }

    (*client)->callbacks = callbacks;
    (*client)->data = callback_data;

    FJ_TRY(client_init(*client, info)) {
        FJ_FREE(&client);
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
fj_err_t client_destroy(fj_client_t * client_)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_wayland_client *)

    client_deinit(client);
    FJ_FREE(&client);

    return FJ_OK;
}


static
fj_err_t client_run(fj_client_t * client_, fj_client_run_type_t run_type, void * _serve_data)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_wayland_client *)
    FJ_ARG_UNUSED(serve_data)

    FJ_INIT_TRY

    if (run_type != FJ_CLIENT_RUN_MAIN) {
        return FJ_OK;
    }

    FJ_TRY(client_init_run(client)) {
        return FJ_RESULT;
    }

    while (!fj_unixpoller_should_finish(&client->unixpoller)) {
        FJ_TRY(client_prepare_poll(client)) {
            return FJ_RESULT;
        }

        FJ_TRY(fj_unixpoller_poll(&client->unixpoller)) {
            client_cancel_poll(client);
            return FJ_RESULT;
        }

        FJ_TRY(client_end_poll(client)) {
            return FJ_RESULT;
        }
    }

    return FJ_OK;
}


static
void client_set_timeout(fj_client_t * client_, fj_seconds_t timeout)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_wayland_client *)

    fj_unixpoller_set_timeout(&client->unixpoller, timeout);
}


static
fj_err_t client_wakeup(fj_client_t * client_)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_wayland_client *)

    return fj_unixpoller_wakeup(&client->unixpoller);
}


struct fj_client_iface const fj_wayland_client_impl = {
    .create = client_create,
    .destroy = client_destroy,
    .run = client_run,
    .set_timeout = client_set_timeout,
    .wakeup = client_wakeup,
};
