#include <src/wayland/client/client.h>

#include <fejix/core/malloc.h>
#include <fejix/core/utils.h>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>


static
void handle_registry_add(
    void * data,
    struct wl_registry * registry,
    uint32_t object_id,
    char const * interface_name,
    uint32_t interface_version
)
{
    FJ_ARG_UNUSED(registry)
    FJ_ARG_FROM_OPAQUE(data, struct fj_wayland_client * this)

    if (fj_streq(interface_name, "wl_compositor")) {
        this->compositor_id = object_id;
        this->compositor_version = interface_version;
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
fj_err_t wayland_dispatch_events(struct fj_wayland_client * this)
{
    this->callback_error = FJ_OK;

    int dispatch_result = wl_display_dispatch_pending(this->display);

    fj_err_t callback_error = this->callback_error;
    this->callback_error = FJ_OK;

    if (dispatch_result == -1) {
        return FJ_ERR_MESSAGE_READ_ERROR;
    }

    return callback_error;
}


static
fj_err_t wayland_handle_poll_event(
    void * callback_data,
    fj_unixpoller_fd_t file_descriptor,
    fj_unixpoller_event_mask_t event_mask
)
{
    FJ_ARG_UNUSED(file_descriptor)
    FJ_ARG_UNUSED(event_mask)
    FJ_ARG_FROM_OPAQUE(callback_data, struct fj_wayland_client * this)

    FJ_WITH_ERRORS

    int result = wl_display_read_events(this->display);

    if (result == -1) {
        return FJ_ERR_MESSAGE_READ_ERROR;
    }

    FJ_TRY(wayland_dispatch_events(this)) {
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
fj_err_t wayland_init(struct fj_wayland_client * this)
{
    FJ_WITH_ERRORS

    this->display = wl_display_connect(NULL);

    if (this->display == NULL) {
        return FJ_ERR_SHELL_CONNECTION_ERROR;
    }

    FJ_TRY(fj_unixpoller_add(
        &this->unixpoller,
        wl_display_get_fd(this->display),
        POLLIN,
        wayland_handle_poll_event
    )) {
        return FJ_RESULT;
    }

    this->registry = wl_display_get_registry(this->display);

    wl_registry_add_listener(this->registry, &registry_listener, this);

    wl_display_dispatch(this->display);
    wl_display_roundtrip(this->display);

    return FJ_OK;
}


static
fj_err_t wayland_deinit(struct fj_wayland_client * this)
{
    wl_display_disconnect(this->display);

    return FJ_OK;
}


static
fj_err_t wayland_prepare_poll(struct fj_wayland_client * this)
{
    FJ_WITH_ERRORS

    while (wl_display_prepare_read(this->display) != 0) {
        FJ_TRY(wayland_dispatch_events(this)) {
            return FJ_RESULT;
        }
    }

    int result = wl_display_flush(this->display);

    if (result == -1) {
        // XXX we do not support incremental display flushing
        return FJ_ERR_MESSAGE_SEND_ERROR;
    }

    return FJ_OK;
}



static
fj_err_t client_begin_poll_iteration(struct fj_wayland_client * this)
{
    return wayland_prepare_poll(this);
}


static
fj_err_t client_end_poll_iteration(struct fj_wayland_client * this)
{
    FJ_WITH_ERRORS

    FJ_TRY(this->callbacks->idle(this->data)) {
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
void client_cancel_poll_iteration(struct fj_wayland_client * this)
{
    wl_display_cancel_read(this->display);
}



static
fj_err_t client_init(struct fj_wayland_client * this, struct fj_client_info const * info)
{
    FJ_WITH_ERRORS

    FJ_TRY(fj_strdup(info->name, &this->name)) {
        return FJ_RESULT;
    }

    FJ_TRY(fj_unixpoller_init(&this->unixpoller, (void *) this)) {
        FJ_FREE(&this->name);
        return FJ_RESULT;
    }

    FJ_TRY(wayland_init(this)) {
        FJ_FREE(&this->name);
        fj_unixpoller_deinit(&this->unixpoller);
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
void client_deinit(struct fj_wayland_client * this)
{
    wayland_deinit(this);
    fj_unixpoller_deinit(&this->unixpoller);
    FJ_FREE(&this->name);
}


static
fj_err_t client_create(
    fj_client_t */*? out*/ * _this,
    struct fj_client_callbacks const * callbacks,
    void * callback_data,
    struct fj_client_info const * info
)
{
    FJ_ARG_FROM_OPAQUE(_this, struct fj_wayland_client * * this)

    FJ_WITH_ERRORS

    FJ_TRY(FJ_ALLOC_ZEROED(this)) {
        return FJ_RESULT;
    }

    (*this)->callbacks = callbacks;
    (*this)->data = callback_data;

    FJ_TRY(client_init(*this, info)) {
        FJ_FREE(&this);
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
fj_err_t client_destroy(fj_client_t * _this)
{
    FJ_ARG_FROM_OPAQUE(_this, struct fj_wayland_client * this)

    client_deinit(this);
    FJ_FREE(&this);

    return FJ_OK;
}


static
fj_err_t client_run(fj_client_t * _this, fj_client_run_type_t run_type, void * serve_data)
{
    FJ_ARG_UNUSED(serve_data)
    FJ_ARG_FROM_OPAQUE(_this, struct fj_wayland_client * this)

    FJ_WITH_ERRORS

    if (run_type != FJ_CLIENT_RUN_TYPE_MAIN) {
        return FJ_OK;
    }

    FJ_TRY(this->callbacks->idle(this->data)) {
        return FJ_RESULT;
    }

    while (!fj_unixpoller_should_finish(&this->unixpoller)) {
        FJ_TRY(client_begin_poll_iteration(this)) {
            return FJ_RESULT;
        }

        FJ_TRY(fj_unixpoller_poll(&this->unixpoller)) {
            client_cancel_poll_iteration(this);
            return FJ_RESULT;
        }

        FJ_TRY(client_end_poll_iteration(this)) {
            return FJ_RESULT;
        }
    }

    return FJ_OK;
}


static
void client_set_timeout(fj_client_t * _this, fj_seconds_t timeout)
{
    FJ_ARG_FROM_OPAQUE(_this, struct fj_wayland_client * this)

    fj_unixpoller_set_timeout(&this->unixpoller, timeout);
}


static
fj_err_t client_wakeup(fj_client_t * _this)
{
    FJ_ARG_FROM_OPAQUE(_this, struct fj_wayland_client * this)

    return fj_unixpoller_wakeup(&this->unixpoller);
}


struct fj_client_iface const fj_wayland_client_impl = {
    .create = client_create,
    .destroy = client_destroy,
    .run = client_run,
    .set_timeout = client_set_timeout,
    .wakeup = client_wakeup,
};
