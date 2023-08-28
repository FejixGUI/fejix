#include "src/internal_prelude.h"
#include "src/modules/x11/client.h"


static fj_err_t connect_x11(struct fj_x11_data * x11_data)
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


static void disconnect_x11(struct fj_x11_data * x11_data)
{
    XCloseDisplay(x11_data->xdisplay);
}


static fj_err_t init_x11(struct fj_client * client)
{
    struct fj_x11_data * x11_data = fj_alloc_zeroed(sizeof(struct fj_x11_data));

    if (x11_data == NULL) {
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    connect_x11(x11_data);

    client->platform_data.x11_data = x11_data;
    return FJ_OK;
}


static void destroy_x11(struct fj_client * client)
{
    disconnect_x11(client->platform_data.x11_data);
    fj_free(client->platform_data.x11_data);
}


static fj_err_t run_x11(struct fj_client * client)
{
    (void) client;
    return FJ_OK;
}


fj_err_t fj_x11_run(struct fj_client * client)
{
    fj_err_t err = FJ_OK;

    err = init_x11(client);
    if (err != FJ_OK) {
        return err;
    }

    err = run_x11(client);
    destroy_x11(client);
    return err;
}
