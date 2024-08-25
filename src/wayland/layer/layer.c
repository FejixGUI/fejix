#include <src/wayland/layer/layer.h>

#include <fejix/core/utils.h>
#include <fejix/core/alloc.h>


static
fj_err_t layer_iface_init(
    fj_client_t * client_,
    struct fj_layer_callbacks const * callbacks
)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_wayland_client *)

    if (client->compositor_id == 0) {
        return FJ_OK;
    }

    client->layer_callbacks = *callbacks;

    client->compositor = wl_registry_bind(
        client->registry,
        client->compositor_id,
        &wl_compositor_interface,
        client->compositor_version
    );

    return FJ_OK;
}


static
fj_err_t layer_iface_create(
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
    (*layer)->surface = wl_compositor_create_surface(client->compositor);

    if ((*layer)->surface == NULL) {
        FJ_FREE(layer);
    }

    return FJ_OK;
}


static
fj_err_t layer_iface_destroy(fj_client_t * _client, fj_layer_t * layer_)
{
    FJ_ARG_UNUSED(client)
    FJ_ARG_FROM_OPAQUE(layer, struct fj_wayland_layer *)

    wl_surface_destroy(layer->surface);
    FJ_FREE(&layer);

    return FJ_OK;
}


static
fj_err_t layer_iface_update(
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


struct fj_layer_iface const fj_wayland_layer_impl = {
    .init = layer_iface_init,
    .create = layer_iface_create,
    .destroy = layer_iface_destroy,
    .update = layer_iface_update,
};
