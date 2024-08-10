#ifndef FEJIX_WM_H_
#define FEJIX_WM_H_


#include <fejix/interface/client.h>
#include <fejix/interface/canvas.h>


FJ_DEFINE_OPAQUE_TYPE(fj_wm_t)
FJ_DEFINE_OPAQUE_TYPE(fj_wm_layer_t)
FJ_DEFINE_OPAQUE_TYPE(fj_wm_view_t)


typedef uint32_t fj_wm_layer_info_update_flags_t;

enum fj_wm_layer_info_update_flags {
    FJ_WM_LAYER_INFO_UPDATE_FLAG_PRESENT_INFO = (1<<0),
    FJ_WM_LAYER_INFO_UPDATE_FLAG_MAPPING_INFO = (1<<1),
    FJ_WM_LAYER_INFO_UPDATE_FLAG_BLENDING_INFO = (1<<2),
    FJ_WM_LAYER_INFO_UPDATE_FLAG_OPTIMIZATION_INFO = (1<<2),
};

typedef uint32_t fj_wm_layer_sync_flags_t;

enum fj_wm_layer_sync_flags {
    FJ_WM_LAYER_SYNC_FLAG_SYNC = (1<<0),
    FJ_WM_LAYER_SYNC_FLAG_ASYNC = (1<<1),
    FJ_WM_LAYER_SYNC_FLAG_DESYNC = (1<<2),
};

typedef uint32_t fj_wm_layer_present_type_t;

enum fj_wm_layer_present_type {
    FJ_WM_LAYER_PRESENT_TYPE_VISIBLE = (1<<0),
    FJ_WM_LAYER_PRESENT_TYPE_INVISIBLE = (1<<1),
    FJ_WM_LAYER_PRESENT_TYPE_INVALID = (1<<2),
};

typedef uint32_t fj_wm_layer_orientation_t;

enum fj_wm_layer_orientation {
    FJ_WM_LAYER_ORIENTATION_NORMAL = (1<<0),
    FJ_WM_LAYER_ORIENTATION_ROTATED90 = (1<<1),
    FJ_WM_LAYER_ORIENTATION_ROTATED180 = (1<<2),
    FJ_WM_LAYER_ORIENTATION_ROTATED270 = (1<<3),
    FJ_WM_LAYER_ORIENTATION_VFLIPPED = (1<<4),
    FJ_WM_LAYER_ORIENTATION_VFLIPPED_ROTATED90 = (1<<5),
    FJ_WM_LAYER_ORIENTATION_VFLIPPED_ROTATED180 = (1<<6),
    FJ_WM_LAYER_ORIENTATION_VFLIPPED_ROTATED270 = (1<<7),
};

typedef uint32_t fj_wm_layer_interpolation_type_t;

enum fj_wm_layer_interpolation_type {
    FJ_WM_LAYER_INTERPOLATION_TYPE_UNKNOWN = (1<<0),
    FJ_WM_LAYER_INTERPOLATION_TYPE_NEAREST = (1<<1),
    FJ_WM_LAYER_INTERPOLATION_TYPE_LINEAR = (1<<2),
};

typedef uint32_t fj_wm_layer_mapping_flags_t;

enum fj_wm_layer_mapping_flags {
    FJ_WM_LAYER_MAPPING_FLAG_CROP = (1<<0),
    FJ_WM_LAYER_MAPPING_FLAG_SCALE = (1<<1),
    FJ_WM_LAYER_MAPPING_FLAG_ORIENT = (1<<2),
    FJ_WM_LAYER_MAPPING_FLAG_CROP_IDENTITY = (1<<3),
    FJ_WM_LAYER_MAPPING_FLAG_SCALE_IDENTITY = (1<<4),
    FJ_WM_LAYER_MAPPING_FLAG_ORIENT_IDENTITY = (1<<5),
};

typedef uint32_t fj_wm_layer_alpha_type_t;

enum fj_wm_layer_alpha_type {
    FJ_WM_LAYER_ALPHA_TYPE_IGNORED = (1<<0),
    FJ_WM_LAYER_ALPHA_TYPE_PREMUL = (1<<1),
};

typedef uint32_t fj_wm_layer_alpha_flags_t;

enum fj_wm_layer_alpha_flags {
    FJ_WM_LAYER_ALPHA_FLAG_FACTOR = (1<<0),
    FJ_WM_LAYER_ALPHA_FLAG_OPAQUE = (1<<1),
    FJ_WM_LAYER_ALPHA_FLAG_TRANSPARENT = (1<<2),
    FJ_WM_LAYER_ALPHA_FLAG_FACTOR_OPAQUE = (1<<3),
    FJ_WM_LAYER_ALPHA_FLAG_FACTOR_TRANSPARENT = (1<<4),
};


struct fj_wm_layer_present_info {
    fj_wm_layer_t */*?*/ target;
    uint32_t zindex;
    fj_wm_layer_sync_flags_t sync_flags;
    fj_wm_layer_present_type_t present_type;
};

struct fj_wm_layer_present_desc {
    uint32_t max_layer_nesting_depth;
    fj_wm_layer_sync_flags_t sync_flags;
    fj_wm_layer_present_type_t present_types;
};

struct fj_wm_layer_mapping_info {
    struct fj_rect2d source;
    struct fj_viewport2d destination;
    fj_wm_layer_orientation_t orientation;
    fj_wm_layer_interpolation_type_t interpolation_type;
    fj_wm_layer_mapping_flags_t mapping_flags;
};

struct fj_wm_layer_mapping_desc {
    fj_wm_layer_orientation_t orientations;
    fj_wm_layer_interpolation_type_t interpolation_types;
    fj_wm_layer_mapping_flags_t mapping_flags;
};

struct fj_wm_layer_blending_info {
    fj_wm_layer_alpha_type_t alpha_type;
    fj_wm_layer_alpha_flags_t alpha_flags;
    float alpha_factor;
};

struct fj_wm_layer_blending_desc {
    fj_wm_layer_alpha_type_t alpha_types;
    fj_wm_layer_alpha_flags_t alpha_flags;
};

struct fj_wm_layer_optimization_info {
    struct fj_rect2d const */*[]?*/ update_rects;
    struct fj_rect2d const */*[]?*/ opaque_rects;
    uint32_t update_rect_count;
    uint32_t opaque_rect_count;
};

struct fj_wm_layer_optimization_desc {
    uint32_t max_update_rect_count;
    uint32_t max_opaque_rect_count;
};

struct fj_wm_layer_info {
    struct fj_wm_layer_present_info present_info;
    struct fj_wm_layer_mapping_info mapping_info;
    struct fj_wm_layer_blending_info blending_info;
    struct fj_wm_layer_optimization_info optimization_info;
};

struct fj_wm_layer_desc {
    struct fj_wm_layer_present_desc present_desc;
    struct fj_wm_layer_mapping_desc mapping_desc;
    struct fj_wm_layer_blending_desc blending_desc;
    struct fj_wm_layer_optimization_desc optimization_desc;
};


struct fj_wm_callbacks {
    void (* set_view_mode)(void * data);
};

struct fj_wm {
    fj_err_t (* get)(
        fj_wm_t */*? out*/ * this,
        struct fj_wm_callbacks const * callbacks,
        fj_client_t * client
    );

    fj_err_t (* release)(fj_wm_t * this);

    fj_err_t (* get_layer_desc)(fj_wm_t * this, struct fj_wm_layer_desc const */*out*/ * desc);

    fj_err_t (* get_view_desc)(fj_wm_t * this, struct fj_wm_view_desc const */*out*/ * desc);

    /** To really create layer, commit the changes. */
    fj_err_t (* create_layer)(
        fj_wm_t * this,
        fj_wm_layer_t */*? out*/ * layer,
        fj_canvas_t * canvas,
        struct fj_wm_layer_info const * info
    );

    fj_err_t (* destroy_layer)(fj_wm_t * this, fj_wm_layer_t * layer);

    /** Commit the layer at least once to use this function. */
    fj_err_t (* update_layer_info)(
        fj_wm_t * this,
        fj_wm_layer_t * layer,
        struct fj_wm_layer_info const * info,
        fj_wm_layer_info_update_flags_t info_update_flags
    );

    fj_err_t (* create_view)(
        fj_wm_t * this,
        fj_wm_view_t */*? out*/ * view,
        struct fj_wm_view_info const * info
    );

    fj_err_t (* destroy_view)(fj_wm_t * this, fj_wm_view_t * view);

    fj_err_t (* set_layer)(fj_wm_t * this, fj_wm_view_t * view, fj_wm_layer_t * layer);

    fj_err_t (* commit)(fj_wm_t * this, fj_wm_view_t *const */*[]*/ views, uint32_t view_count);
};


#endif
