#include <src/wayland/softer_canvas/softer_canvas.h>

#include <fejix/core/utils.h>
#include <fejix/core/alloc.h>


struct wayland_shm_format_event {
    uint32_t wayland_format;
};


/** Returns COLOR_MAX for unknown formats. */
static
fj_softer_pixel_format_t convert_pixel_format(uint32_t wayland_format)
{
    switch (wayland_format) {
        case WL_SHM_FORMAT_RGB888:
            return FJ_SOFTER_PIXEL_FORMAT_RGB24|FJ_SOFTER_PIXEL_FORMAT_LITTLE_ENDIAN;
        case WL_SHM_FORMAT_BGR888:
            return FJ_SOFTER_PIXEL_FORMAT_RGB24;
        case WL_SHM_FORMAT_RGBX8888:
            return FJ_SOFTER_PIXEL_FORMAT_RGBX32|FJ_SOFTER_PIXEL_FORMAT_LITTLE_ENDIAN;
        case WL_SHM_FORMAT_XBGR8888:
            return FJ_SOFTER_PIXEL_FORMAT_RGBX32;
        case WL_SHM_FORMAT_XRGB8888:
            return FJ_SOFTER_PIXEL_FORMAT_XRGB32|FJ_SOFTER_PIXEL_FORMAT_LITTLE_ENDIAN;
        case WL_SHM_FORMAT_BGRX8888:
            return FJ_SOFTER_PIXEL_FORMAT_XRGB32;
        default: return FJ_SOFTER_PIXEL_FORMAT_COLOR_MAX;
    }
}


static
fj_err_t handle_init_event(
    struct fj_wayland_client * client,
    struct fj_wayland_event_wrapper const * wrapper
)
{
    (void) wrapper;

    FJ_INIT_TRY

    struct fj_softer_canvas_caps const caps = {
        .pixel_format_count = client->softer->pixel_formats.length,
        .pixel_formats = client->softer->pixel_formats.items,
    };

    FJ_TRY(client->softer->callbacks.init(client->data, &caps)) {
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
fj_err_t handle_format_event(
    struct fj_wayland_client * client,
    struct fj_wayland_event_wrapper const * wrapper
)
{
    FJ_INIT_TRY

    struct wayland_shm_format_event const * event = wrapper->event;

    fj_softer_pixel_format_t format = convert_pixel_format(event->wayland_format);

    if (format == FJ_SOFTER_PIXEL_FORMAT_COLOR_MAX) {
        return FJ_OK;
    }

    FJ_TRY(fj_vec_push(&client->softer->pixel_formats, &format)) {
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
void wayland_shm_format(
    void * _client,
    struct wl_shm * shm,
    uint32_t format
)
{
    struct fj_wayland_client * client = (void *) _client;

    FJ_INIT_TRY

    struct wayland_shm_format_event event = {
        .wayland_format = format,
    };

    struct fj_wayland_event_wrapper event_wrapper = {
        .event = &event,
        .event_size = sizeof(event),
        .event_group = FJ_WAYLAND_EVENT_GROUP_INTERFACE_INIT,
        .event_source = (struct wl_proxy *) shm,
        .handle = handle_format_event,
    };

    FJ_TRY(fj_wayland_record_event(client, &event_wrapper)) {
        fj_wayland_record_fail(client);
        return;
    }
}


static
struct wl_shm_listener wayland_shm_listener = {
    .format = wayland_shm_format
};


static
fj_err_t softer_canvas_init_global_data(struct fj_wayland_client * client)
{
    FJ_INIT_TRY

    fj_vec_init(&client->softer->pixel_formats, sizeof(fj_softer_pixel_format_t));

    wl_shm_add_listener(client->softer->shm, &wayland_shm_listener, client);

    struct fj_wayland_event_wrapper const event_wrapper = {
        .event = NULL,
        .event_size = 0,
        .event_group = FJ_WAYLAND_EVENT_GROUP_INTERFACE_INIT,
        .event_source = (struct wl_proxy *) client->softer->shm,
        .handle = handle_init_event,
    };

    FJ_TRY(fj_wayland_record_event(client, &event_wrapper)) {
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
fj_err_t softer_canvas_init(
    fj_client_t * _client,
    struct fj_softer_canvas_callbacks const * callbacks
)
{
    struct fj_wayland_client * client = (void *) _client;

    FJ_INIT_TRY

    struct fj_wayland_global const * shm_global =
        fj_wayland_get_static_global(client, FJ_WAYLAND_INTERFACE_SHM);

    if (shm_global == NULL) {
        return callbacks->init(client->data, NULL);
    }

    FJ_TRY(FJ_ALLOC_ZEROED(&client->softer)) {
        return FJ_RESULT;
    }

    client->softer->callbacks = *callbacks;

    FJ_TRY(fj_wayland_bind_global(
        client,
        FJ_WAYLAND_INTERFACE_SHM,
        shm_global,
        (void *) &client->softer->shm
    )) {
        return FJ_RESULT;
    }

    FJ_TRY(softer_canvas_init_global_data(client)) {
        FJ_FREE(&client->softer);
        return FJ_RESULT;
    }

    return FJ_OK;
}


fj_err_t softer_canvas_deinit(fj_client_t * _client)
{
    struct fj_wayland_client * client = (void *) _client;

    if (fj_vec_has_allocated(&client->softer->pixel_formats)) {
        fj_vec_deinit(&client->softer->pixel_formats);
    }

    FJ_FREE(&client->softer);

    return FJ_OK;
}


static
fj_err_t softer_canvas_create(
    fj_client_t * client,
    fj_softer_canvas_t */*? out*/ * canvas,
    struct fj_softer_canvas_info const * info
)
{

}


static
fj_err_t softer_canvas_destroy(fj_client_t * client, fj_softer_canvas_t * canvas)
{

}


static
fj_err_t softer_canvas_update(
    fj_client_t * client,
    fj_softer_canvas_t * canvas,
    struct fj_softer_canvas_info const * info
)
{

}


static
fj_err_t softer_canvas_present(fj_client_t * client, fj_softer_canvas_t * canvas)
{

}


struct fj_softer_canvas_iface const fj_wayland_softer_canvas_iface = {
    .init = softer_canvas_init,
    .deinit = softer_canvas_deinit,
    .create = softer_canvas_create,
    .destroy = softer_canvas_destroy,
    .update = softer_canvas_update,
    .present = softer_canvas_present,
};
