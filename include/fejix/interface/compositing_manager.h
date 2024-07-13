#ifndef FEJIX_COMPOSITING_MANAGER_H_
#define FEJIX_COMPOSITING_MANAGER_H_


#if 0

#include <fejix/interface/server.h>
#include <fejix/interface/rendering_manager.h>
#include <fejix/interface/display_manager.h>


FJ_DEFINE_UNIQUE_TYPE(fj_compositing_manager_t)
FJ_DEFINE_UNIQUE_TYPE(fj_compositing_context_t)


struct fj_compositing_desc {
    uint32_t max_compositing_depth;
};


struct fj_compositing_presentation_info {
    fj_rendering_context_t * source;

    union {
        fj_display_context_t * display_context;
        fj_compositing_context_t * compositing_context;
    } destination;

    fj_enum32_t destination_type;

    fj_enum32_t presentation_mode;

    uint32_t z_index;
};

struct fj_compositing_transform_info {
    struct fj_rect2d source_rect;
    struct fj_viewport2d destination_viewport;
    fj_enum32_t orientation;
};

struct fj_compositing_blending_info {
    fj_enum32_t alpha_mode;
    float alpha_factor;
};

struct fj_compositing_optimizations_info {
    struct fj_rect2d const */*[]?*/ update_rects;
    struct fj_rect2d const */*[]?*/ opaque_rects;
    uint32_t update_count;
    uint32_t opaque_count;
};

struct fj_compositing_context_info {
    struct fj_compositing_presentation_info presentation;
    struct fj_compositing_transform_info transform;
    struct fj_compositing_blending_info blending;
    struct fj_compositing_optimizations_info optimizations;
};


struct fj_compositing_manager {
    fj_err_t (* create)(
        fj_compositing_manager_t */*?*/ /*out*/ * manager,
        fj_server_t * server
    );

    fj_err_t (* destroy)(
        fj_compositing_manager_t * manager
    );

    fj_err_t (* get_compositing_desc)(
        fj_compositing_manager_t * manager,
        struct fj_compositing_desc const * /*out*/ * desc
    );

    fj_err_t (* create_context)(
        fj_compositing_manager_t * manager,
        fj_compositing_context_t */*?*/ /*out*/ * context,
        struct fj_compositing_context_info const * info
    );

    fj_err_t (* update_context)(
        fj_compositing_manager_t * manager,
        fj_compositing_context_t * context,
        struct fj_compositing_context_info const * info
    );

    fj_err_t (* destroy_context)(
        fj_compositing_manager_t * manager,
        fj_compositing_context_t * context
    );

    fj_err_t (* commit)(
        fj_compositing_manager_t * manager
    );
};

#endif


#endif
