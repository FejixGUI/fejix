#ifndef FEJIX_BLANK_H_
#define FEJIX_BLANK_H_


#include <fejix/interface/canvas.h>
#include <fejix/interface/client.h>


struct fj_blank_iface {
    fj_err_t (* init)(fj_client_t * client);

    fj_err_t (* create)(fj_client_t * client, fj_canvas_t */*? out*/ * blank_canvas);

    fj_err_t (* destroy)(fj_client_t * client, fj_canvas_t * blank_canvas);

    fj_err_t (* update)(fj_client_t * client, fj_canvas_t * blank_canvas);
};


#endif
