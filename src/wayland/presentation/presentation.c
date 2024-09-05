#include <src/wayland/presentation/presentation.h>

#include <fejix/core/utils.h>
#include <fejix/core/alloc.h>


static
fj_err_t presentation_handle_init(
    struct fj_wayland_client * client,
    struct fj_wayland_event_wrapper const * event_wrapper
)
{
    (void) event_wrapper;

    FJ_INIT_TRY

    struct fj_presentation_caps presentation_caps = {
        .flags = FJ_PRESENTATION_SYNC,
    };

    FJ_TRY(client->presentation->callbacks.init(client->data, &presentation_caps)) {
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
fj_err_t presentation_data_init(struct fj_wayland_client * client)
{
    FJ_INIT_TRY

    struct fj_wayland_global const * compositor_global =
        fj_wayland_get_static_global(client, FJ_WAYLAND_INTERFACE_COMPOSITOR);

    FJ_TRY(fj_wayland_bind_global(
        client,
        FJ_WAYLAND_INTERFACE_COMPOSITOR,
        compositor_global,
        (void *) &client->presentation->compositor
    )) {
        return FJ_RESULT;
    }

    struct fj_wayland_event_wrapper const event_wrapper = {
        .event = NULL,
        .event_size = 0,
        .event_group = FJ_WAYLAND_EVENT_GROUP_INTERFACE_INIT,
        .event_source = (struct wl_proxy *) client->presentation->compositor,
        .handle = presentation_handle_init,
    };

    FJ_TRY(fj_wayland_record_event(client, &event_wrapper)) {
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
fj_err_t presentation_init(fj_client_t * _client, struct fj_presentation_callbacks const * callbacks)
{
    struct fj_wayland_client * client = (void *) _client;

    FJ_INIT_TRY

    struct fj_wayland_global const * compositor =
        fj_wayland_get_static_global(client, FJ_WAYLAND_INTERFACE_COMPOSITOR);

    if (compositor == NULL) {
        return callbacks->init(client->data, NULL);
    }

    FJ_TRY(FJ_ALLOC_ZEROED(&client->presentation)) {
        return FJ_RESULT;
    }

    client->presentation->callbacks = *callbacks;

    FJ_TRY(presentation_data_init(client)) {
        FJ_FREE(&client->presentation);
        return FJ_RESULT;
    }

    return FJ_OK;
}


fj_err_t presentation_deinit(fj_client_t * _client)
{
    struct fj_wayland_client * client = (void *) _client;

    FJ_FREE(&client->presentation);

    return FJ_OK;
}



static
fj_err_t presentation_create(
    fj_client_t * _client,
    fj_presentation_t */*out*/ * _presentation,
    fj_canvas_base_t * _canvas,
    struct fj_presentation_info const * info
)
{
    struct fj_wayland_client * client = (void *) _client;
    struct fj_wayland_presentation ** presentation = (void *) _presentation;
    struct fj_wayland_canvas_base * canvas = (void *) _canvas;

    FJ_INIT_TRY

    FJ_TRY(FJ_ALLOC_ZEROED(presentation)) {
        return FJ_RESULT;
    }

    (*presentation)->canvas = canvas;
    (*presentation)->flags = info->flags;
    (*presentation)->size = info->size;
    (*presentation)->surface = wl_compositor_create_surface(client->presentation->compositor);

    if ((*presentation)->surface == NULL) {
        FJ_FREE(presentation);
        return FJ_ERR_REQUEST_FAILED;
    }

    return FJ_OK;
}


static
fj_err_t presentation_destroy(fj_client_t * _client, fj_presentation_t * _presentation)
{
    struct fj_wayland_client * client = (void *) _client;
    struct fj_wayland_presentation * presentation = (void *) _presentation;
    (void) client;

    wl_surface_destroy(presentation->surface);
    FJ_FREE(&presentation);

    return FJ_OK;
}


static
fj_err_t presentation_update(
    fj_client_t * _client,
    fj_presentation_t * _presentation,
    struct fj_presentation_info const * presentation_info,
    fj_presentation_update_flags_t update_flags
)
{
    struct fj_wayland_client * client = (void *) _client;
    struct fj_wayland_presentation * presentation = (void *) _presentation;
    (void) update_flags;
    (void) client;

    presentation->size = presentation_info->size;
    presentation->flags = presentation_info->flags;

    return FJ_OK;
}


struct fj_presentation_iface const fj_wayland_presentation_iface = {
    .init = presentation_init,
    .deinit = presentation_deinit,
    .create = presentation_create,
    .destroy = presentation_destroy,
    .update = presentation_update,
};
