#include "src/internal_prelude.h"
#include "src/modules/x11/client.h"


static fj_err_t x11_connect(struct fj_x11_data * x11_data)
{
    x11_data->xdisplay = XOpenDisplay(NULL);

    if (x11_data->xdisplay == NULL) {
        return FJ_ERR("cannot connect to X11 display");
    }

    x11_data->connection = XGetXCBConnection(x11_data->xdisplay);

    if (x11_data->connection == NULL) {
        return FJ_ERR("cannot create XCB connection");
    }

    return FJ_OK;
}


static void x11_disconnect(struct fj_x11_data * x11_data)
{
    XCloseDisplay(x11_data->xdisplay);
}


static fj_err_t x11_setup(struct fj_client * client)
{
    struct fj_x11_data * x11_data = fj_alloc_zeroed(sizeof(struct fj_x11_data));

    if (x11_data == NULL) {
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    fj_err_t err = x11_connect(x11_data);
    if (err != FJ_OK) {
        fj_free(x11_data);
        return err;
    }

    client->platform_data.x11_data = x11_data;


    return FJ_OK;
}


static fj_err_t x11_shutdown(struct fj_client * client)
{
    x11_disconnect(client->platform_data.x11_data);
    fj_free(client->platform_data.x11_data);
    return FJ_OK;
}


static fj_err_t x11_client_run(struct fj_client * client)
{
    (void) client;
    return FJ_OK;
}


static fj_err_t x11_client_setup(struct fj_client * client)
{
    fj_err_t err = FJ_OK;

    err = x11_setup(client);
    if (err != FJ_OK) {
        return err;
    }

    err = client->client_listener->init(client);
    if (err != FJ_OK) {
        x11_shutdown(client);
        return err;
    }

    return FJ_OK;
}


static fj_err_t x11_client_shutdown(struct fj_client * client)
{
    fj_err_t err = FJ_OK;

    err = client->client_listener->release(client);
    if (err != FJ_OK) {
        x11_shutdown(client);
        return err;
    }

    return x11_shutdown(client);
}


fj_err_t fj_x11_client_run(struct fj_client * client, fj_idstring_t id)
{
    fj_err_t err = FJ_OK;

    err = x11_client_setup(client);
    if (err != FJ_OK) {
        return err;
    }

    err = x11_client_run(client);
    if (err != FJ_OK) {
        // TODO Is shutdown the best strategy on failure?
        x11_client_shutdown(client);
        return err;
    }
    
    return x11_client_shutdown(client);
}
