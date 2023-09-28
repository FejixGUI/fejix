#include <src/winapi/client.h>
#include <src/winapi/utils.h>

#include <fejix/malloc.h>
#include <fejix/utils.h>


FJ_REQUIRE_VERSION(fj_client_listener, v_0_2)


static fj_err_t client_core_init(struct fj_client * client)
{
    HINSTANCE app_instance = GetModuleHandle(NULL);
    client->data.winapi->app_instance = app_instance;

    return FJ_OK;
}


static fj_err_t client_core_deinit(struct fj_client * client)
{
    (void) client;
    return FJ_OK;
}


fj_err_t client_data_init(struct fj_client * client)
{
    fj_err_t err = client_core_init(client);

    if (err != FJ_OK) {
        return err;
    }

    // TODO Init modules

    return FJ_OK;
}


static fj_err_t client_data_deinit(struct fj_client * client)
{
    // TODO Deinit modules

    fj_err_t err = client_core_deinit(client);

    if (err != FJ_OK) {
        return err;
    }

    return FJ_OK;
}


static fj_err_t client_init(struct fj_client * client)
{
    struct fj_winapi_data * winapi_data = fj_alloc_zeroed(sizeof *winapi_data);

    if (winapi_data == NULL) {
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    client->data.winapi = winapi_data;

    fj_err_t err = client_data_init(client);

    if (err != FJ_OK) {
        return err;
    }

    return FJ_OK;
}


static fj_err_t client_deinit(struct fj_client * client)
{
    if (client->data.winapi == NULL) {
        return NULL;
    }

    fj_err_t err = client_data_deinit(client);

    fj_free(client->data.winapi);

    return err;
}


static fj_err_t client_eventloop_run(struct fj_client * client)
{
    // TODO Event loop

    (void) client;
    return FJ_OK;
}


static fj_err_t client_run(struct fj_client * client)
{
    fj_err_t err = client->client_listener->init(client);

    if (err != FJ_OK) {
        return err;
    }

    err = client_eventloop_run(client);

    return client->client_listener->deinit(client, err);
}


fj_err_t fj_winapi_client_run(struct fj_client * client)
{
    fj_err_t err = client_init(client);

    if (err != FJ_OK) {
        client_deinit(client);
        return err;
    }

    err = client_run(client);

    if (err != FJ_OK) {
        client_deinit(client);
        return err;
    }

    return client_deinit(client);
}
