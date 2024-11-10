#ifndef FEJIX_INTERFACE_WINDOW_H_
#define FEJIX_INTERFACE_WINDOW_H_


#include <fejix/interface/client.h>


struct fj_window_manager;
struct fj_window_builder;
struct fj_window FJ_PUBLICLY({ union fj_tag tag; });


struct fj_window_create_info {
    union fj_tag tag;
    struct fj_canvas *canvas;
};

struct fj_window_info {
    struct fj_density2d density;
    struct fj_size2d size;
    fj_orientation_type_t orientation;
};

struct fj_window_callbacks {
    fj_err_t (*update_window)(
        struct fj_client *client,
        struct fj_window *window,
        struct fj_window_info const *window_info
    );
};

struct fj_window_funcs {
    fj_err_t (*create_manager)(
        struct fj_window_manager **manager,
        struct fj_client *client,
        struct fj_window_callbacks const *callbacks
    );

    fj_err_t (*destroy_manager)(struct fj_window_manager *manager);

    fj_err_t (*create_builder)(
        struct fj_window_manager *manager,
        struct fj_window_builder **builder,
        struct fj_window_create_info const *window_info
    );

    /**
        May result into undefined behavior if there are still other builders referencing this
        builder.
    */
    fj_err_t (*destroy_builder)(
        struct fj_window_manager *manager,
        struct fj_window_builder *builder
    );

    /** The window only becomes effective when the builder gets destroyed. */
    fj_err_t (*create_window)(
        struct fj_window_manager *manager,
        struct fj_window **window,
        struct fj_window_builder *builder
    );

    fj_err_t (*destroy_window)(struct fj_window_manager *manager, struct fj_window *window);

    fj_err_t (*update_windows)(
        struct fj_window_manager *manager,
        struct fj_window *const *windows,
        uint32_t window_count
    );
};


#endif
