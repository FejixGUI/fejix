#ifndef FEJIX_WM_H_
#define FEJIX_WM_H_


#include "fejix/core/base.h"
#include <fejix/interface/client.h>
#include <fejix/interface/canvas.h>


FJ_DEFINE_OPAQUE_TYPE(fj_wm_t)
FJ_DEFINE_OPAQUE_TYPE(fj_wm_layer_t)
FJ_DEFINE_OPAQUE_TYPE(fj_wm_view_t)


enum fj_wm_layer_operation {
    FJ_WM_LAYER_OPERATION_COMPOSITE,
    FJ_WM_LAYER_OPERATION_BLEND,
    FJ_WM_LAYER_OPERATION_CROP,
    FJ_WM_LAYER_OPERATION_SCALE,
    FJ_WM_LAYER_OPERATION_ROTATE,
};


struct fj_wm_layer_present_info {
    fj_enum32_t present_mode;
    fj_enum32_t sync_flags;
};

struct fj_wm_layer_composite_info {
    fj_wm_layer_t */*?*/ target;
    struct fj_position2d position;
    uint32_t zindex;
};

struct fj_wm_layer_crop_info {
    struct fj_rect2d rect;
    fj_bool32_t identity;
};

struct fj_wm_layer_scale_info {
    struct fj_size2d size;
    fj_bool32_t identity;
};

struct fj_wm_layer_blend_info {
    fj_enum32_t alpha_mode;
    float alpha_factor;
    struct fj_rect2d const */*[]?*/ opaque_rects;
    uint32_t opaque_rect_count;
};

struct fj_wm_layer_info {
    fj_enum32_t operations;
    struct fj_wm_layer_present_info present;
    struct fj_wm_layer_composite_info composite;
    struct fj_wm_layer_crop_info crop;
    struct fj_wm_layer_scale_info scale;
    struct fj_wm_layer_blend_info blend;
};


struct fj_wm {
    fj_err_t (* get)(fj_wm_t */*? out*/ * this, fj_client_t * client);

    fj_err_t (* release)(fj_wm_t * this);

    fj_err_t (* create_layer)(
        fj_wm_t * this,
        fj_wm_layer_t */*? out*/ * context,
        fj_canvas_t source
    );

    fj_err_t (* destroy_layer)(fj_wm_t * this, fj_wm_layer_t * context);

    fj_err_t (* get_layer_desc)(fj_wm_t * this, struct fj_wm_layer_desc const * /*out*/ * desc);

    fj_err_t (* set_layer_info)(
        fj_wm_t * this,
        fj_wm_layer_t * layer,
        struct fj_wm_layer_info const * info
    );

    fj_err_t (* create_view)(
        fj_wm_t * this,
        fj_wm_view_t */*? out*/ * view,
        fj_wm_layer_t * layer,
        struct fj_wm_view_info const * info
    );

    fj_err_t (* destroy_view)(fj_wm_t * this, fj_wm_view_t * view);

    fj_err_t (* update)(fj_wm_t * this, fj_wm_view_t *const */*[]*/ views, uint32_t view_count);
};


#endif
