#include <src/wayland/softer_canvas/softer_canvas.h>

#include <fejix/core/utils.h>
#include <fejix/core/alloc.h>


struct wayland_shm_format_event {
    uint32_t wayland_format;
};


/** Returns COLOR_MAX for unsupported formats. */
static
fj_softer_pixel_format_t to_pixel_format(uint32_t wayland_format)
{
    switch (wayland_format) {
        case WL_SHM_FORMAT_RGB888:
            return FJ_SOFTER_PIXEL_FORMAT_RGB24
                | FJ_SOFTER_PIXEL_FORMAT_LITTLE_ENDIAN;
        case WL_SHM_FORMAT_BGR888:
            return FJ_SOFTER_PIXEL_FORMAT_RGB24;
        case WL_SHM_FORMAT_RGBX8888:
            return FJ_SOFTER_PIXEL_FORMAT_RGBX32
                | FJ_SOFTER_PIXEL_FORMAT_LITTLE_ENDIAN;
        case WL_SHM_FORMAT_XBGR8888:
            return FJ_SOFTER_PIXEL_FORMAT_RGBX32;
        case WL_SHM_FORMAT_XRGB8888:
            return FJ_SOFTER_PIXEL_FORMAT_XRGB32
                | FJ_SOFTER_PIXEL_FORMAT_LITTLE_ENDIAN;
        case WL_SHM_FORMAT_BGRX8888:
            return FJ_SOFTER_PIXEL_FORMAT_XRGB32;
        default: return FJ_SOFTER_PIXEL_FORMAT_COLOR_MAX;
    }
}

/** Returns UINT32_MAX for unsupported formats. */
static
uint32_t from_pixel_format(fj_softer_pixel_format_t format)
{
    switch (format) {
        case FJ_SOFTER_PIXEL_FORMAT_RGB24
            | FJ_SOFTER_PIXEL_FORMAT_LITTLE_ENDIAN:
            return WL_SHM_FORMAT_RGB888;
        case FJ_SOFTER_PIXEL_FORMAT_RGB24:
            return WL_SHM_FORMAT_BGR888;
        case FJ_SOFTER_PIXEL_FORMAT_RGBX32
            | FJ_SOFTER_PIXEL_FORMAT_LITTLE_ENDIAN:
            return WL_SHM_FORMAT_RGBX8888;
        case FJ_SOFTER_PIXEL_FORMAT_RGBX32:
            return WL_SHM_FORMAT_XBGR8888;
        case FJ_SOFTER_PIXEL_FORMAT_XRGB32
            | FJ_SOFTER_PIXEL_FORMAT_LITTLE_ENDIAN:
            return WL_SHM_FORMAT_XRGB8888;
        case FJ_SOFTER_PIXEL_FORMAT_XRGB32:
            return WL_SHM_FORMAT_BGRX8888;
        default: return UINT32_MAX;
    }
}


static
fj_err_t handle_init_event(
    struct fj_wayland_client * client,
    struct fj_wayland_event_wrapper const * wrapper
)
{
    (void) wrapper;

        struct fj_softer_canvas_caps const caps = {
        .pixel_format_count = client->softer->pixel_formats.length,
        .pixel_formats = client->softer->pixel_formats.items,
    };

    FJ_TRY(client->softer->callbacks.init(client->data, &caps)) {
        return fj_result;
    }

    return FJ_OK;
}


static
fj_err_t handle_format_event(
    struct fj_wayland_client * client,
    struct fj_wayland_event_wrapper const * wrapper
)
{
    struct wayland_shm_format_event const * event = wrapper->event;

    fj_softer_pixel_format_t format = to_pixel_format(event->wayland_format);

    if (format == FJ_SOFTER_PIXEL_FORMAT_COLOR_MAX) {
        return FJ_OK;
    }

    FJ_TRY(fj_vec_push(&client->softer->pixel_formats, &format)) {
        return fj_result;
    }

    return FJ_OK;
}


static
void wayland_shm_format(void * _client, struct wl_shm * shm, uint32_t format)
{
    struct fj_wayland_client * client = (void *) _client;

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
        return fj_result;
    }

    return FJ_OK;
}


static
fj_err_t softer_canvas_init(
    struct fj_client * _client,
    struct fj_softer_canvas_callbacks const * callbacks
)
{
    struct fj_wayland_client * client = (void *) _client;

        struct fj_wayland_global const * shm_global =
        fj_wayland_get_static_global(client, FJ_WAYLAND_INTERFACE_SHM);

    if (shm_global == NULL) {
        return callbacks->init(client->data, NULL);
    }

    FJ_TRY(FJ_ALLOC_ZEROED(&client->softer)) {
        return fj_result;
    }

    client->softer->callbacks = *callbacks;

    FJ_TRY(fj_wayland_bind_global(
        client,
        FJ_WAYLAND_INTERFACE_SHM,
        shm_global,
        (void *) &client->softer->shm
    )) {
        return fj_result;
    }

    FJ_TRY(softer_canvas_init_global_data(client)) {
        FJ_FREE(&client->softer);
        return fj_result;
    }

    return FJ_OK;
}


static
fj_err_t softer_canvas_deinit(struct fj_client * _client)
{
    struct fj_wayland_client * client = (void *) _client;

    if (fj_vec_has_allocated(&client->softer->pixel_formats)) {
        fj_vec_deinit(&client->softer->pixel_formats);
    }

    FJ_FREE(&client->softer);

    return FJ_OK;
}


static
size_t softer_canvas_get_stride(struct fj_wayland_softer_canvas const * canvas)
{
    return (size_t) canvas->info.size.width * fj_softer_get_pixel_size(canvas->info.pixel_format);
}


static
size_t softer_canvas_get_data_size(struct fj_wayland_softer_canvas const * canvas)
{
    return softer_canvas_get_stride(canvas) * canvas->info.size.height;
}


static
fj_err_t softer_canvas_create_buffer(
    struct fj_wayland_client * client,
    struct fj_wayland_softer_canvas * canvas,
    struct fj_wayland_softer_canvas_buffer * buffer
)
{
    size_t size = softer_canvas_get_data_size(canvas);
    size_t stride = softer_canvas_get_stride(canvas);

    FJ_TRY(fj_unixshm_alloc(&buffer->shm, size)) {
        return fj_result;
    }

    buffer->pool = wl_shm_create_pool(
        client->softer->shm,
        buffer->shm.file,
        (int32_t) buffer->shm.size
    );

    if (buffer->pool == NULL) {
        fj_unixshm_free(&buffer->shm);
        return FJ_ERR_REQUEST_FAILED;
    }

    buffer->buffer = wl_shm_pool_create_buffer(
        buffer->pool,
        0,
        (int32_t) canvas->info.size.width,
        (int32_t) canvas->info.size.height,
        (int32_t) stride,
        from_pixel_format(canvas->info.pixel_format)
    );

    if (buffer->buffer == NULL) {
        fj_unixshm_free(&buffer->shm);
        return FJ_ERR_REQUEST_FAILED;
    }

    buffer->available = true;

    return FJ_OK;
}


static
fj_err_t softer_canvas_destroy_buffer(struct fj_wayland_softer_canvas_buffer * buffer)
{
    FJ_TRY(fj_unixshm_unref(&buffer->shm)) {
        return fj_result;
    }

    wl_buffer_destroy(buffer->buffer);
    wl_shm_pool_destroy(buffer->pool);

    return FJ_OK;
}


static
fj_err_t softer_canvas_resize_buffer(
    struct fj_wayland_softer_canvas * canvas,
    struct fj_wayland_softer_canvas_buffer * buffer
)
{
    FJ_TRY(fj_unixshm_realloc(&buffer->shm, softer_canvas_get_data_size(canvas))) {
        return fj_result;
    }

    wl_buffer_destroy(buffer->buffer);

    wl_shm_pool_resize(buffer->pool, (int32_t) buffer->shm.size);

    buffer->buffer = wl_shm_pool_create_buffer(
        buffer->pool,
        0,
        (int32_t) canvas->info.size.width,
        (int32_t) canvas->info.size.height,
        (int32_t) softer_canvas_get_stride(canvas),
        from_pixel_format(canvas->info.pixel_format)
    );

    if (buffer->buffer == NULL) {
        return FJ_ERR_REQUEST_FAILED;
    }

    return FJ_OK;
}


static
fj_err_t softer_canvas_create_buffers(
    struct fj_wayland_client * client,
    struct fj_wayland_softer_canvas * canvas
)
{
    FJ_TRY(softer_canvas_create_buffer(client, canvas, &canvas->buffers[0])) {
        return fj_result;
    }

    FJ_TRY(softer_canvas_create_buffer(client, canvas, &canvas->buffers[1])) {
        softer_canvas_destroy_buffer(&canvas->buffers[0]);
        return fj_result;
    }

    return FJ_OK;
}


static
fj_err_t softer_canvas_create(
    struct fj_client * _client,
    fj_softer_canvas_t */*? out*/ * _canvas,
    struct fj_softer_canvas_info const * info
)
{
    struct fj_wayland_client * client = (void *) _client;
    struct fj_wayland_softer_canvas * * canvas = (void *) _canvas;

        FJ_TRY(FJ_ALLOC_ZEROED(canvas)) {
        return fj_result;
    }

    (*canvas)->info = *info;

    FJ_TRY(softer_canvas_create_buffers(client, *canvas)) {
        FJ_FREE(canvas);
        return fj_result;
    }

    return FJ_OK;
}


static
fj_err_t softer_canvas_destroy(struct fj_client * _client, fj_softer_canvas_t * _canvas)
{
    (void) _client;

    struct fj_wayland_softer_canvas * canvas = (void *) _canvas;

        FJ_TRY(softer_canvas_destroy_buffer(&canvas->buffers[0])) {
        return fj_result;
    }

    FJ_TRY(softer_canvas_destroy_buffer(&canvas->buffers[1])) {
        return fj_result;
    }

    FJ_FREE(&canvas);

    return FJ_OK;
}


static
fj_bool32_t filter_canvas_buffer_release_events(
    struct fj_wayland_client * client,
    void * _canvas,
    struct fj_wayland_event_wrapper const * wrapper
)
{
    (void) client;

    struct fj_wayland_softer_canvas * canvas = (void *) _canvas;

    for (uint32_t i=0; i<FJ_ARRAY_LEN(canvas->buffers); i++) {
        if (wrapper->event_source == (struct wl_proxy *) canvas->buffers[i].buffer) {
            return true;
        }
    }

    return false;
}


static
fj_bool32_t softer_canvas_info_changed(
    struct fj_wayland_softer_canvas const * canvas,
    struct fj_softer_canvas_info const * info
)
{
    return canvas->info.size.width == info->size.width
        && canvas->info.size.height == info->size.height;
}


static
fj_err_t softer_canvas_wait_for_available_buffers(
    struct fj_wayland_client * client,
    struct fj_wayland_softer_canvas * canvas
)
{
    while (canvas->available_buffer_count == 0) {
        FJ_TRY(fj_wayland_wait_for_events(client)) {
            return fj_result;
        }

        FJ_TRY(fj_wayland_handle_events(client, canvas, filter_canvas_buffer_release_events)) {
            return fj_result;
        }
    }

    return FJ_OK;
}


static
fj_err_t softer_canvas_update(
    struct fj_client * _client,
    fj_softer_canvas_t * _canvas,
    struct fj_softer_canvas_info const * info
)
{
    struct fj_wayland_client * client = (void *) _client;
    struct fj_wayland_softer_canvas * canvas = (void *) _canvas;

        if (!softer_canvas_info_changed(canvas, info)) {
        return FJ_OK;
    }

    canvas->info = *info;

    FJ_TRY(softer_canvas_wait_for_available_buffers(client, canvas)) {
        return fj_result;
    }

    struct fj_wayland_softer_canvas_buffer * buffer = &canvas->buffers[canvas->active_buffer];

    FJ_TRY(softer_canvas_resize_buffer(canvas, buffer)) {
        return fj_result;
    }

    struct fj_softer_canvas_desc desc = {
        .pixel_data = buffer->shm.data,
        .stride = softer_canvas_get_stride(canvas),
    };

    FJ_TRY(client->softer->callbacks.update(client->data, (void *) canvas, &desc)) {
        return fj_result;
    }

    return FJ_OK;
}


static
fj_err_t softer_canvas_present(struct fj_client * _client, fj_softer_canvas_t * _canvas)
{
    struct fj_wayland_client * client = (void *) _client;
    struct fj_wayland_softer_canvas * canvas = (void *) _canvas;

        FJ_TRY(softer_canvas_wait_for_available_buffers(client, canvas)) {
        return fj_result;
    }

    struct fj_wayland_softer_canvas_buffer * buffer = &canvas->buffers[canvas->active_buffer];
    struct fj_wayland_output * output = canvas->base.output;

    buffer->available = false;
    canvas->available_buffer_count--;

    wl_surface_attach(output->surface, buffer->buffer, 0, 0);
    wl_surface_commit(output->surface);

    return FJ_OK;
}


struct fj_softer_canvas_iface const fj_wayland_softer_canvas_iface = {
    .init = softer_canvas_init,
    .deinit = softer_canvas_deinit,
    .create = softer_canvas_create,
    .destroy = softer_canvas_destroy,
    .update = softer_canvas_update,
    .present = softer_canvas_present,
};
