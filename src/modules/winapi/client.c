#include <fejix/core/malloc.h>
#include <fejix/core/utils.h>

#include "src/client.h"
#include "src/modules/winapi/client.h"
#include "src/modules/winapi/utils.h"


FJ_REQUIRE_VERSION(fj_client_listener, v_0_1)


static void client_init_app_instance(struct fj_client * client)
{
    HINSTANCE app_instance = GetModuleHandle(NULL);
    client->data.winapi->app_instance = app_instance;
}


static fj_err_t client_init_window_class_name(struct fj_client * client)
{
    LPWSTR window_class_name = fj_winapi_utf8_to_wstr(client->client_id);

    if (window_class_name == NULL) {
        return FJ_ERR("client identifier string is invalid");
    }

    client->data.winapi->window_class_name = window_class_name;

    return FJ_OK;
}


static fj_err_t client_init(struct fj_client * client)
{
    client_init_app_instance(client);

    fj_err_t err = client_init_window_class_name(client);

    if (err != FJ_OK) {
        return err;
    }

    return FJ_OK;
}


static fj_err_t client_destroy(struct fj_client * client)
{
    fj_free(client->data.winapi->window_class_name);

    return FJ_OK;
}


static fj_err_t client_data_init(struct fj_client * client)
{
    struct fj_winapi_data * winapi_data = fj_alloc_zeroed(sizeof *winapi_data);

    if (winapi_data == NULL) {
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    client->data.winapi = winapi_data;

    fj_err_t err = client_init(client);

    if (err != FJ_OK) {
        fj_free(winapi_data);
        return err;
    }

    // TODO Init modules and call module callbacks

    return FJ_OK;
}


static fj_err_t client_data_deinit(struct fj_client * client)
{
    fj_err_t err = client_destroy(client);
    fj_free(client->data.winapi);
    return err;
}


static fj_err_t client_eventloop_run(struct fj_client * client)
{
    // TODO event loop
    (void) client;
    return FJ_OK;
}


static fj_err_t client_data_run(struct fj_client * client)
{
    fj_err_t err = client->client_listener->init(client);

    if (err != FJ_OK) {
        return err;
    }

    err = client_eventloop_run(client);

    if (err != FJ_OK) {
        // TODO Add deinit reason
        client->client_listener->deinit(client);
        return err;
    }

    err = client->client_listener->deinit(client);

    if (err != FJ_OK) {
        return err;
    }

    return FJ_OK;
}


fj_err_t fj_winapi_client_run(struct fj_client * client)
{
    fj_err_t err = client_data_init(client);

    if (err != FJ_OK) {
        return err;
    }

    err = client_data_run(client);

    if (err != FJ_OK) {
        client_data_deinit(client);
        return err;
    }

    return client_data_deinit(client);
}
