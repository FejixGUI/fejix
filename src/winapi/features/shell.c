#include <src/winapi/features/shell.h>
#include <src/winapi/client.h>
#include <src/winapi/utils.h>

#include <fejix/utils.h>
#include <fejix/malloc.h>


static fj_err_t client_core_init_window_class_name(struct fj_client * client)
{
    LPWSTR window_class_name = fj_winapi_utf8_to_wstr(client->client_id);

    if (window_class_name == NULL) {
        return FJ_ERR("client identifier string is invalid");
    }

    client->data.winapi->window_class_name = window_class_name;

    return FJ_OK;
}


static fj_err_t client_core_init_window_class(struct fj_client * client)
{
    fj_err_t err = client_core_init_window_class_name(client);

    if (err != FJ_OK) {
        return err;
    }

    // TODO Init window class

    return FJ_OK;
}


static void client_core_deinit_window_class(struct fj_client * client)
{
    // TODO Deinit window class

    if (client->data.winapi->window_class_name == NULL) {
        return;
    }

    fj_free(client->data.winapi->window_class_name);
}
