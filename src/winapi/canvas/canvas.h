#ifndef FEJIX_WINAPI_CANVAS_H
#define FEJIX_WINAPI_CANVAS_H


#include <src/winapi/client/client.h>
#include <src/winapi/window/window.h>


struct fj_canvas {
};


struct fj_canvas_funcs {
    fj_err_t (*create_context)(
        struct fj_client *client,
        struct fj_window *window,
        struct fj_canvas *canvas
    );

    fj_err_t (*destroy_context)(
        struct fj_client *client,
        struct fj_window *window,
        struct fj_canvas *canvas
    );
};


#endif
