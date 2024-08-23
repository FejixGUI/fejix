#ifndef FEJIX_SOFTER_H_
#define FEJIX_SOFTER_H_


#include <fejix/interface/client.h>
#include <fejix/interface/canvas.h>


FJ_DEFINE_OPAQUE_TYPE(fj_softer_t)



struct fj_softer_info {

};

struct fj_softer_desc {

};


struct fj_softer_callbacks {

};

struct fj_softer_iface {
    fj_err_t (* init)(
        fj_client_t * client,
        struct fj_softer_callbacks const * callbacks,
        struct fj_softer_desc /*out*/ * desc
    );

    fj_err_t (* create)(
        fj_client_t * client,
        fj_softer_t */*? out*/ * softer,
        fj_canvas_t */*? out*/ * canvas,
        struct fj_softer_info const * info
    );

    fj_err_t (* destroy)(fj_client_t * client, fj_softer_t * softer);


};


#endif
