#include <src/implementation/wayland/interface/client/client.h>
#include <src/implementation/wayland/interface/client/wayland.h>

#include <fejix/core/malloc.h>
#include <fejix/core/error.h>
#include <fejix/core/utils.h>


static
fj_err_t client_init(
    struct fj_wayland_client_data * client,
    struct fj_client_info const * info
)
{
    FJ_INIT_TRY

    client->callback = info->callback;

    fj_try fj_unixpoller_init(&client->unixpoller, (void *) client);
    fj_else {
        return fj_result;
    }

    fj_try fj_wayland_init(client);
    fj_else {
        return fj_result;
    }

    return FJ_OK;
}


static
void client_deinit(struct fj_wayland_client_data * client)
{
    fj_wayland_deinit(client);
    fj_unixpoller_deinit(&client->unixpoller);
}


static
fj_err_t client_create(
    fj_client_data_t */*?*/ /*out*/ * _client,
    struct fj_client_info const * info
)
{
    FJ_INIT_TRY

    struct fj_wayland_client_data * * client = (void *) _client;

    fj_try fj_alloc_zeroed((void *) client, sizeof(**client));
    fj_else {
        return fj_result;
    }

    fj_try client_init(*client, info);
    fj_else {
        fj_free((void *) client);
        return fj_result;
    }

    return FJ_OK;
}


static
fj_err_t client_destroy(fj_client_data_t * _client)
{
    struct fj_wayland_client_data * client = (void *) _client;

    client_deinit(client);
    fj_free((void *) client);

    return FJ_OK;
}


static
fj_err_t client_serve(
    fj_client_data_t * _client,
    fj_enum32_t serve_type,
    void * serve_data
)
{
    FJ_INIT_TRY

    FJ_UNUSED(serve_data)

    struct fj_wayland_client_data * client = (void *) _client;

    if (serve_type != FJ_CLIENT_SERVE_TYPE_MAIN) {
        return FJ_OK;
    }

    while (true) {
        if (client->unixpoller.timeout < 0.0) {
            break;
        }

        fj_try fj_unixpoller_poll(&client->unixpoller);
        fj_else {
            return fj_result;
        }

        struct fj_message message = {
            .sender_interface_id = FJ_CLIENT,
            .sender_object = client,
            .message_id = FJ_CLIENT_CONTINUE,
            .message_data = NULL
        };

        fj_try client->callback->call(client->callback, &message);
        fj_else {
            return fj_result;
        }
    }

    return FJ_OK;
}


static
void client_set_timeout(
    fj_client_data_t * _client,
    fj_seconds_t timeout
)
{
    struct fj_wayland_client_data * client = (void *) _client;

    client->unixpoller.timeout = timeout;
}


static
void client_get_interrupt_signal(
    fj_client_data_t * _client,
    struct fj_client_interrupt_signal const * /*out*/ * signal
)
{
    struct fj_wayland_client_data * client = (void *) _client;

    *signal = &client->unixpoller.interrupt_signal.interrupt_signal;
}


struct fj_client const fj_wayland_client = {
    .create = client_create,
    .destroy = client_destroy,
    .serve = client_serve,
    .set_timeout = client_set_timeout,
    .get_interrupt_signal = client_get_interrupt_signal,
};
