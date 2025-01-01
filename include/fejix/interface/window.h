#ifndef FEJIX_INTERFACE_WINDOW_H_
#define FEJIX_INTERFACE_WINDOW_H_


#include <fejix/interface/app.h>
#include <fejix/interface/image.h>

#include <fejix/core/geometry.h>


typedef uint32_t fj_window_manager_extension_id_t;

enum fj_window_manager_extension_id {
    FJ_WINDOW_MANAGER_EXTENSION_WINDOW_SYNC_MANAGER,
};


struct fj_window_manager;
FJ_DEFINE_TAGGED_STRUCT(fj_window)

struct fj_window_create_info {
    struct fj_image_usage_context *image_usage_context;
    struct fj_image_set *image_set;
};

struct fj_window_update_info {
    struct fj_density density;
    struct fj_size size;
    fj_orientation_type_t orientation;
};

struct fj_window_manager_callbacks {
    fj_err_t (*on_update_window)(
        struct fj_app *app,
        struct fj_window *window,
        struct fj_window_update_info const *info
    );
};

struct fj_window_manager_funcs {
    void const *(*get_extension)(fj_window_manager_extension_id_t id);

    fj_err_t (*get)(
        struct fj_app *app,
        struct fj_window_manager **out_manager,
        struct fj_window_manager_callbacks const *callbacks
    );

    fj_err_t (*release)(struct fj_window_manager *manager);

    fj_err_t (*get_image_usage_context)(
        struct fj_window_manager *manager,
        struct fj_image_usage_context **out_image_usage_context
    );

    fj_err_t (*create_window)(
        struct fj_window_manager *manager,
        struct fj_window **out_window,
        struct fj_window_create_info const *info
    );

    fj_err_t (*destroy_window)(struct fj_window_manager *manager, struct fj_window *window);

    fj_err_t (*set_window_needs_update)(
        struct fj_window_manager *manager,
        struct fj_window *window
    );

    fj_err_t (*update_windows)(
        struct fj_window_manager *manager,
        struct fj_window *const *windows,
        uint32_t window_count
    );
};


#endif
