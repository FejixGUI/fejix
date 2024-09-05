#ifndef FEJIX_PRESENTATION_H_
#define FEJIX_PRESENTATION_H_


#include <fejix/interface/client.h>


FJ_DEFINE_OPAQUE_TYPE(fj_presentation_t)
FJ_DEFINE_OPAQUE_TYPE(fj_canvas_base_t)


typedef uint32_t fj_presentation_flags_t;

enum fj_presentation_flags {
    FJ_PRESENTATION_HIDDEN = (1<<0),
    FJ_PRESENTATION_ASYNC = (1<<1),
    FJ_PRESENTATION_SYNC = (1<<2),
};

typedef uint32_t fj_presentation_update_flags_t;

enum fj_presentation_update_flags {
    FJ_PRESENTATION_UPDATE_SIZE = (1<<0),
    FJ_PRESENTATION_UPDATE_FLAGS = (1<<1),
    FJ_PRESENTATION_UPDATE_SYNC_REQUEST = (1<<2),
    FJ_PRESENTATION_UPDATE_SYNC_RESPONSE = (1<<3),
};


struct fj_presentation_caps {
    fj_presentation_flags_t flags;
};

struct fj_presentation_info {
    struct fj_size2d size;
    fj_presentation_flags_t flags;
};

struct fj_presentation_callbacks {
    fj_err_t (* init)(void */*?*/ data, struct fj_presentation_caps const */*?*/ caps);

    fj_err_t (* update)(void */*?*/ data, fj_presentation_t * presentation);
};

struct fj_presentation_iface {
    fj_err_t (* init)(
        fj_client_t * client,
        struct fj_presentation_callbacks const * callbacks
    );

    fj_err_t (* deinit)(struct fj_client_t * client);

    fj_err_t (* create)(
        fj_client_t * client,
        fj_presentation_t */*? out*/ * presentation,
        fj_canvas_base_t * canvas,
        struct fj_presentation_info const * presentation_info
    );

    fj_err_t (* destroy)(fj_client_t * client, fj_presentation_t * presentation);

    fj_err_t (* update)(
        fj_client_t * client,
        fj_presentation_t * presentation,
        struct fj_presentation_info const * presentation_info,
        fj_presentation_update_flags_t update_flags
    );
};


#endif
