#ifndef FEJIX_LAYER_H_
#define FEJIX_LAYER_H_


#include <fejix/interface/client.h>
#include <fejix/interface/canvas.h>


FJ_DEFINE_OPAQUE_TYPE(fj_layer_t)


typedef uint32_t fj_layer_flags_t;

enum fj_layer_flags {
    FJ_LAYER_ASYNC = (1<<0),
    FJ_LAYER_SYNC = (1<<1),
    FJ_LAYER_SYNC_DEFERRED = (1<<2),
    FJ_LAYER_SYNC_UNUSED = (1<<3),
    FJ_LAYER_HIDDEN = (1<<4),
};

typedef uint32_t fj_layer_update_flags_t;

enum fj_layer_update_flags {
    FJ_LAYER_UPDATE_SYNC_REQUEST = (1<<0),
    FJ_LAYER_UPDATE_SYNC_RESPONSE = (1<<1),
};


struct fj_layer_desc {
    fj_layer_flags_t flags;
};

struct fj_layer_info {
    struct fj_size2d size;
    fj_layer_flags_t flags;
};

struct fj_layer_callbacks {
    fj_err_t (* sync)(void * data, fj_layer_t * layer);
};

struct fj_layer_iface {
    fj_err_t (* init)(
        fj_client_t * client,
        struct fj_layer_callbacks const * callbacks,
        struct fj_layer_desc /*out*/ * desc
    );

    fj_err_t (* create)(
        fj_client_t * client,
        fj_layer_t */*? out*/ * layer,
        fj_canvas_t * canvas,
        struct fj_layer_info const * info
    );

    fj_err_t (* destroy)(fj_client_t * client, fj_layer_t * layer);

    fj_err_t (* update)(
        fj_client_t * client,
        fj_layer_t * layer,
        struct fj_layer_info const * info,
        fj_layer_update_flags_t update_flags
    );
};


#endif
