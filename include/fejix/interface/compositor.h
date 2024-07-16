#ifndef FEJIX_COMPOSITOR_H_
#define FEJIX_COMPOSITOR_H_


#include <fejix/interface/client.h>
#include <fejix/interface/rendering_manager.h>


typedef struct fj_compositing_context_t fj_compositing_context_t;
typedef struct fj_display_context_t fj_display_context_t;


struct fj_compositing_presentation_info {
    fj_compositing_context_t */*?*/ destination;
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

    struct fj_rect2d const */*[]?*/ opaque_rects;
    uint32_t opaque_rect_count;
};

// struct fj_compositing_optimizations_info {
//     struct fj_rect2d const */*[]?*/ update_rects;
//     uint32_t update_count;
// };

struct fj_compositing_context_info {
    struct fj_compositing_presentation_info presentation;
    struct fj_compositing_transform_info transform;
    struct fj_compositing_blending_info blending;
    // struct fj_compositing_optimizations_info optimizations;
};


struct fj_compositor {
    fj_err_t (* init)(
        fj_client_t * client
    );

    fj_err_t (* destroy)(
        fj_client_t * client
    );

    fj_err_t (* create_compositing_context)(
        fj_client_t * client,
        fj_compositing_context_t */*? out*/ * context,
        fj_rendering_context_t source,
        struct fj_compositing_context_info const * info
    );

    fj_err_t (* destroy_compositing_context)(
        fj_client_t * client,
        fj_compositing_context_t * context
    );

    fj_err_t (* get_compositing_desc)(
        fj_client_t * client,
        fj_compositing_context_t * context,
        struct fj_compositor_desc const * /*out*/ * desc
    );

    fj_err_t (* set_compositing_info)(
        fj_client_t * client,
        fj_compositing_context_t * context,
        struct fj_compositing_context_info const * info
    );

    fj_err_t (* create_display_context)(
        fj_client_t * client,
        fj_display_context_t */*? out*/ * context,
        fj_compositing_context_t * compositing_context,
        struct fj_display_context_info const * info
    );

    fj_err_t (* destroy_display_context)(
        fj_client_t * client,
        fj_display_context_t * context
    );

    fj_err_t (* update)(
        fj_client_t * client,
        fj_display_context_t *const */*[]*/ display_contexts,
        uint32_t display_context_count
    );
};


#endif
