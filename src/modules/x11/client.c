#include "src/internal_prelude.h"
#include "src/modules/x11/client.h"


fj_err_t fj_x11_run(struct fj_client * client)
{
    struct fj_x11_client * x11_client =
        fj_alloc_zeroed(sizeof(struct fj_x11_client));

    if (x11_client == NULL) {
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    x11_client->xdisplay = XOpenDisplay(NULL);

    if (x11_client->xdisplay == NULL) {
        return FJ_ERR("cannot connect to X11 display");
    }

    x11_client->connection = XGetXCBConnection(x11_client->xdisplay);

    if (x11_client->connection == NULL) {
        return FJ_ERR("cannot create XCB connection");
    }


    client->client_data.x11 = x11_client;

    return FJ_OK;
}


fj_err_t fj_x11_client_destroy(struct fj_client * client)
{
    XCloseDisplay(client->client_data.x11->xdisplay);
    fj_free(client->client_data.x11);
    return FJ_OK;
}
