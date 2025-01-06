#include <src/wayland/output/output.h>

#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>


static fj_err_t output_handle_init(
    struct fj_client *client,
    struct fj_wayland_event_wrapper const *event_wrapper
)
{
    (void) event_wrapper;

    struct fj_output_caps output_caps = {
        .flags = FJ_OUTPUT_SYNC,
    };

    FJ_TRY (client->output->callbacks.init(client->data, &output_caps)) {
        return fj_result;
    }

    return FJ_OK;
}


static fj_err_t output_data_init(struct fj_client *client)
{
    struct fj_wayland_global const *compositor_global = fj_wayland_get_static_global(
        client, FJ_WAYLAND_INTERFACE_COMPOSITOR
    );

    FJ_TRY (fj_wayland_bind_global(
                client,
                FJ_WAYLAND_INTERFACE_COMPOSITOR,
                compositor_global,
                (void *) &client->output->compositor
            )) {
        return fj_result;
    }

    struct fj_wayland_event_wrapper const event_wrapper = {
        .event = NULL,
        .event_size = 0,
        .event_group = FJ_WAYLAND_EVENT_GROUP_INTERFACE_INIT,
        .event_source = (struct wl_proxy *) client->output->compositor,
        .handle = output_handle_init,
    };

    FJ_TRY (fj_wayland_record_event(client, &event_wrapper)) {
        return fj_result;
    }

    return FJ_OK;
}


static fj_err_t output_init(struct fj_client *_client, struct fj_output_callbacks const *callbacks)
{
    struct fj_client *client = (void *) _client;

    struct fj_wayland_global const *compositor = fj_wayland_get_static_global(
        client, FJ_WAYLAND_INTERFACE_COMPOSITOR
    );

    if (compositor == NULL) {
        return callbacks->init(client->data, NULL);
    }

    FJ_TRY (FJ_ALLOC_ZEROED(&client->output)) {
        return fj_result;
    }

    client->output->callbacks = *callbacks;

    FJ_TRY (output_data_init(client)) {
        FJ_FREE(&client->output);
        return fj_result;
    }

    return FJ_OK;
}


fj_err_t output_deinit(struct fj_client *_client)
{
    struct fj_client *client = (void *) _client;

    FJ_FREE(&client->output);

    return FJ_OK;
}


static fj_err_t output_create(
    struct fj_client *_client,
    struct fj_output * /*out*/ *_output,
    fj_canvas_base_t *_canvas,
    struct fj_output_info const *info
)
{
    struct fj_client *client = (void *) _client;
    struct fj_wayland_output **output = (void *) _output;
    struct fj_wayland_canvas_base *canvas = (void *) _canvas;

    FJ_TRY (FJ_ALLOC_ZEROED(output)) {
        return fj_result;
    }

    (*output)->canvas = canvas;
    (*output)->info = *info;
    (*output)->surface = wl_compositor_create_surface(client->output->compositor);

    if ((*output)->surface == NULL) {
        FJ_FREE(output);
        return FJ_ERR_REQUEST_FAILED;
    }

    canvas->output = *output;

    return FJ_OK;
}


static fj_err_t output_destroy(struct fj_client *_client, struct fj_output *_output)
{
    struct fj_client *client = (void *) _client;
    struct fj_wayland_output *output = (void *) _output;
    (void) client;

    wl_surface_destroy(output->surface);
    FJ_FREE(&output);

    return FJ_OK;
}


static fj_err_t output_update(
    struct fj_client *_client,
    struct fj_output *_output,
    struct fj_output_info const *output_info,
    fj_output_update_flags_t update_flags
)
{
    struct fj_client *client = (void *) _client;
    struct fj_wayland_output *output = (void *) _output;
    (void) update_flags;
    (void) client;

    output->info = *output_info;

    return FJ_OK;
}


struct fj_output_iface const fj_wayland_output_iface = {
    .init = output_init,
    .deinit = output_deinit,
    .create = output_create,
    .destroy = output_destroy,
    .update = output_update,
};
