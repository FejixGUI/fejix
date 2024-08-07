#include <src/wayland/client/client.h>

#include <fejix/core/malloc.h>
#include <fejix/core/error.h>
#include <fejix/core/utils.h>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>


static
void handle_registry_add(
    void * data,
    struct wl_registry * registry,
    uint32_t object_id,
    const char * interface_name,
    uint32_t interface_version
)
{
    FJ_ARG_UNUSED(registry)
    FJ_ARG_FROM_OPAQUE(data, struct fj_wayland_client * client)

    if (fj_streq(FJ_UTF8(interface_name), FJ_UTF8(wl_compositor_interface.name))) {
        client->compositor_id = object_id;
        client->compositor_version = interface_version;
    }
}


static
void handle_registry_remove(void * data, struct wl_registry * registry, uint32_t object_id)
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
fj_err_t wayland_handle_poll_event(
    void * callback_data,
    fj_unixpoller_fd_t file_descriptor,
    fj_unixpoller_event_mask_t event_mask
)
{
    FJ_ARG_UNUSED(file_descriptor)
    FJ_ARG_UNUSED(event_mask)
    FJ_ARG_FROM_OPAQUE(callback_data, struct fj_wayland_client * client)

    int result;

    result = wl_display_read_events(client->display);

    if (result == -1) {
        return FJ_ERR_MESSAGE_READ_ERROR;
    }

    result = wl_display_dispatch_pending(client->display);

    if (result == -1) {
        return FJ_ERR_MESSAGE_READ_ERROR;
    }

    return FJ_OK;
}


static
fj_err_t wayland_init(struct fj_wayland_client * client)
{
    FJ_WITH_ERRORS

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
        return FJ_RESULT;
    }

    client->registry = wl_display_get_registry(client->display);

    wl_registry_add_listener(client->registry, &registry_listener, client);

    wl_display_dispatch(client->display);
    wl_display_roundtrip(client->display);

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
    int result;

    while (wl_display_prepare_read(client->display) != 0) {
        result = wl_display_dispatch_pending(client->display);

        if (result == -1) {
            return FJ_ERR_MESSAGE_READ_ERROR;
        }
    }

    result = wl_display_flush(client->display);

    if (result == -1) {
        // TODO handle incremental display flushing
        return FJ_ERR_MESSAGE_SEND_ERROR;
    }

    return FJ_OK;
}



static
fj_err_t client_begin_poll_iteration(struct fj_wayland_client * client)
{
    return wayland_prepare_poll(client);
}


static
fj_err_t client_end_poll_iteration(struct fj_wayland_client * client)
{
    FJ_WITH_ERRORS

    struct fj_message const message = {
        .sender_interface_id = FJ_CLIENT,
        .sender_object = client,
        .message_id = FJ_CLIENT_CONTINUE,
        .message_data = NULL
    };

    FJ_TRY(client->callback->call(client->callback, &message)) {
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
void client_cancel_poll_iteration(struct fj_wayland_client * client)
{
    wl_display_cancel_read(client->display);
}



static
fj_err_t client_init(struct fj_wayland_client * client, struct fj_client_info const * info)
{
    FJ_WITH_ERRORS

    client->callback = info->callback;

    FJ_TRY(fj_unixpoller_init(&client->unixpoller, (void *) client)) {
        return FJ_RESULT;
    }

    FJ_TRY(wayland_init(client)) {
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
}


static
fj_err_t client_create(
    fj_client_t */*? out*/ * _client,
    struct fj_client_info const * info
)
{
    FJ_ARG_FROM_OPAQUE(_client, struct fj_wayland_client * * client)

    FJ_WITH_ERRORS

    FJ_TRY(fj_alloc_zeroed_auto(client)) {
        return FJ_RESULT;
    }

    FJ_TRY(client_init(*client, info)) {
        fj_free_auto(&client);
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
fj_err_t client_destroy(fj_client_t * _client)
{
    FJ_ARG_FROM_OPAQUE(_client, struct fj_wayland_client * client)

    client_deinit(client);
    fj_free_auto(&client);

    return FJ_OK;
}


static
fj_err_t client_run(fj_client_t * _client, fj_enum32_t serve_type, void * serve_data)
{
    FJ_ARG_UNUSED(serve_data)
    FJ_ARG_FROM_OPAQUE(_client, struct fj_wayland_client * client)

    FJ_WITH_ERRORS

    if (serve_type != FJ_CLIENT_RUN_TYPE_MAIN) {
        return FJ_OK;
    }

    while (!fj_unixpoller_should_finish(&client->unixpoller)) {
        FJ_TRY(client_begin_poll_iteration(client)) {
            return FJ_RESULT;
        }

        FJ_TRY(fj_unixpoller_poll(&client->unixpoller)) {
            client_cancel_poll_iteration(client);
            return FJ_RESULT;
        }

        FJ_TRY(client_end_poll_iteration(client)) {
            return FJ_RESULT;
        }
    }

    return FJ_OK;
}


static
void client_set_timeout(fj_client_t * _client, fj_seconds_t timeout)
{
    FJ_ARG_FROM_OPAQUE(_client, struct fj_wayland_client * client)

    client->unixpoller.timeout = timeout;
}


static
struct fj_client_waker const * client_get_interrupt_signal(fj_client_t * _client)
{
    FJ_ARG_FROM_OPAQUE(_client, struct fj_wayland_client * client)

    return FJ_INTO_BASE_PTR(&client->unixpoller.waker);
}


struct fj_client const fj_wayland_client = {
    .create = client_create,
    .destroy = client_destroy,
    .run = client_run,
    .set_timeout = client_set_timeout,
    .get_waker = client_get_interrupt_signal,
};
