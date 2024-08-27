#include <src/wayland/layer/layer.h>

#include <fejix/core/utils.h>
#include <fejix/core/alloc.h>


static
fj_err_t layer_handle_init(
    struct fj_wayland_client * client,
    struct fj_wayland_event_wrapper const * _event_wrapper
)
{
    FJ_ARG_UNUSED(event_wrapper)

    FJ_INIT_TRY

    struct fj_layer_caps layer_caps = {
        .flags = FJ_LAYER_SYNC,
    };

    FJ_TRY(client->layer->callbacks.init(client->data, &layer_caps)) {
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
fj_err_t layer_data_init(
    struct fj_wayland_client * client,
    struct fj_wayland_global const * global
)
{
    FJ_INIT_TRY

    FJ_TRY(fj_wayland_bind_global(
        client,
        FJ_WAYLAND_INTERFACE_COMPOSITOR,
        global,
        (void **) &client->layer->compositor
    )) {
        return FJ_RESULT;
    }

    struct fj_wayland_event_wrapper const event_wrapper = {
        .event = NULL,
        .event_size = 0,
        .event_group = FJ_WAYLAND_EVENT_GROUP_INTERFACE_INIT,
        .event_source = (struct wl_proxy *) &client->layer->compositor,
        .handle = layer_handle_init,
    };

    FJ_TRY(fj_wayland_record_event(client, &event_wrapper)) {
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
fj_err_t layer_init(fj_client_t * client_, struct fj_layer_callbacks const * callbacks)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_wayland_client *)

    FJ_INIT_TRY

    struct fj_wayland_global const * global;

    if (!fj_wayland_get_static_global(client, FJ_WAYLAND_INTERFACE_COMPOSITOR, &global)) {
        return callbacks->init(client->data, NULL);
    }

    FJ_TRY(FJ_ALLOC_ZEROED(&client->layer)) {
        return FJ_RESULT;
    }

    client->layer->callbacks = *callbacks;

    FJ_TRY(layer_data_init(client, global)) {
        FJ_FREE(&client->layer);
        return FJ_RESULT;
    }

    return FJ_OK;
}


fj_err_t fj_wayland_layer_cleanup(struct fj_wayland_client * client)
{
    if (client->layer == NULL) {
        return FJ_OK;
    }

    FJ_FREE(&client->layer);

    return FJ_OK;
}



static
fj_err_t layer_create(
    fj_client_t * client_,
    fj_layer_t */*out*/ * layer_,
    fj_canvas_t * canvas,
    struct fj_layer_info const * info
)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_wayland_client *)
    FJ_ARG_FROM_OPAQUE(layer, struct fj_wayland_layer */*out*/ *)

    FJ_INIT_TRY

    FJ_TRY(FJ_ALLOC_ZEROED(layer)) {
        return FJ_RESULT;
    }

    (*layer)->canvas = canvas;
    (*layer)->flags = info->flags;
    (*layer)->size = info->size;
    (*layer)->surface = wl_compositor_create_surface(client->layer->compositor);

    if ((*layer)->surface == NULL) {
        FJ_FREE(layer);
        return FJ_ERR_REQUEST_FAILED;
    }

    return FJ_OK;
}


static
fj_err_t layer_destroy(fj_client_t * _client, fj_layer_t * layer_)
{
    FJ_ARG_UNUSED(client)
    FJ_ARG_FROM_OPAQUE(layer, struct fj_wayland_layer *)

    wl_surface_destroy(layer->surface);
    FJ_FREE(&layer);

    return FJ_OK;
}


static
fj_err_t layer_update(
    fj_client_t * _client,
    fj_layer_t * layer_,
    struct fj_layer_info const * info,
    fj_layer_update_flags_t _update_flags
)
{
    FJ_ARG_UNUSED(client)
    FJ_ARG_FROM_OPAQUE(layer, struct fj_wayland_layer *)
    FJ_ARG_UNUSED(update_flags)

    layer->size = info->size;
    layer->flags = info->flags;

    return FJ_OK;
}


struct fj_layer_iface const fj_wayland_layer_iface = {
    .init = layer_init,
    .create = layer_create,
    .destroy = layer_destroy,
    .update = layer_update,
};
