#ifndef FEJIX_INTERFACE_WINDOW_H_
#define FEJIX_INTERFACE_WINDOW_H_


#include <fejix/interface/canvas.h>
#include <fejix/interface/client.h>


struct fj_window_manager;
struct fj_window FJ_PUBLICLY({ union fj_tag tag; });


struct fj_window_manager_desc {
    // TODO
};

struct fj_window_info {
    union fj_tag tag;
    struct fj_canvas *canvas;
};

struct fj_window_desc {
    // TODO
};

struct fj_window_callbacks {
    fj_err_t (*update)(
        struct fj_client *client,
        struct fj_window *window,
        struct fj_window_desc const *window_desc
    );
};

struct fj_window_funcs {
    fj_err_t (*create_manager)(
        struct fj_window_manager **manager,
        struct fj_window_manager_desc *manager_desc,
        struct fj_client *client,
        struct fj_window_callbacks const *callbacks
    );

    fj_err_t (*destroy_manager)(struct fj_window_manager *manager);

    fj_err_t (*create_window)(
        struct fj_window_manager *manager,
        struct fj_window **window,
        struct fj_window_info const *info
    );

    fj_err_t (*destroy_window)(struct fj_window_manager *manager, struct fj_window *window);

    fj_err_t (*update_window)(struct fj_window_manager *manager, struct fj_window *window);

    fj_err_t (*commit)(struct fj_window_manager *manager);
};


#endif
