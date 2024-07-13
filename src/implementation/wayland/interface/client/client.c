#include <fejix/interface/client.h>

#include <src/implementation/wayland/interface/client/client.h>

#include <src/shared/unixpoller/unixpoller.h>

#include <fejix/core/malloc.h>
#include <fejix/core/error.h>
#include <fejix/core/utils.h>


static
fj_err_t wayland_client_init(
    struct fj_wayland_client_data * client,
    struct fj_client_info const * info
)
{
    FJ_INIT_TRY

    FJ_TRY
    fj_unixpoller_init(&client->unixpoller, (void *) client);

    FJ_ELSE {
        return FJ_RESULT;
    }

    client->callback = info->callback;

    return FJ_OK;
}


static
void wayland_client_deinit(
    struct fj_wayland_client_data * client
)
{
    fj_unixpoller_deinit(&client->unixpoller);
}


static
fj_err_t wayland_client_create(
    fj_client_data_t */*?*/ /*out*/ * _client,
    struct fj_client_info const * info
)
{
    FJ_UNUSED(info)

    FJ_INIT_TRY

    struct fj_wayland_client_data * * client = (void *) _client;

    FJ_TRY
    fj_alloc_zeroed((void *) client, sizeof(**client));

    FJ_ELSE {
        return FJ_RESULT;
    }

    FJ_TRY
    wayland_client_init(*client, info);

    FJ_ELSE {
        fj_free((void *) client);
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
fj_err_t wayland_client_destroy(
    fj_client_data_t * _client
)
{
    struct fj_wayland_client_data * client = (void *) _client;
    wayland_client_deinit(client);
    fj_free((void *) client);

    return FJ_OK;
}


static
fj_err_t wayland_client_serve(
    fj_client_data_t * _client,
    fj_enum32_t serve_type,
    void * serve_data
)
{
    FJ_UNUSED(serve_data)

    FJ_INIT_TRY

    struct fj_wayland_client_data * client = (void *) _client;

    if (serve_type != FJ_CLIENT_SERVE_TYPE_MAIN) {
        return FJ_OK;
    }

    while (true) {
        if (client->unixpoller.timeout < 0.0) {
            break;
        }

        FJ_TRY
        fj_unixpoller_poll(&client->unixpoller);

        FJ_ELSE {
            return FJ_RESULT;
        }

        struct fj_message message = {
            .sender_interface_id = FJ_CLIENT,
            .sender_object = client,
            .message_id = FJ_CLIENT_CONTINUE,
            .message_data = NULL
        };

        FJ_TRY
        client->callback->call(client->callback, _client, &message);

        FJ_ELSE {
            return FJ_RESULT;
        }
    }

    return FJ_OK;
}


static
void wayland_client_set_timeout(
    fj_client_data_t * _client,
    fj_seconds_t timeout
)
{
    struct fj_wayland_client_data * client = (void *) _client;

    client->unixpoller.timeout = timeout;
}


static
void wayland_client_get_interrupt_signal(
    fj_client_data_t * _client,
    struct fj_client_interrupt_signal const * /*out*/ * signal
)
{
    struct fj_wayland_client_data * client = (void *) _client;

    *signal = &client->unixpoller.interrupt_signal.interrupt_signal;
}


struct fj_client fj_wayland_client = {
    .create = wayland_client_create,
    .destroy = wayland_client_destroy,
    .serve = wayland_client_serve,
    .set_timeout = wayland_client_set_timeout,
    .get_interrupt_signal = wayland_client_get_interrupt_signal,
};
