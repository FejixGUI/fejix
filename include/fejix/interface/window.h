#ifndef FEJIX_INTERFACE_WINDOW_H_
#define FEJIX_INTERFACE_WINDOW_H_


#include <fejix/interface/client.h>

#include <fejix/core/geometry.h>


enum fj_window_manager_request_id {
    FJ_WINDOW_MANAGER_ALLOC,
    FJ_WINDOW_MANAGER_DEALLOC,
    FJ_WINDOW_MANAGER_INIT,
    FJ_WINDOW_MANAGER_DEINIT,

    FJ_WINDOW_MANAGER_UPDATE_WINDOWS,
};

enum fj_window_request_id {
    FJ_WINDOW_ALLOC,
    FJ_WINDOW_DEALLOC,
    FJ_WINDOW_INIT,
    FJ_WINDOW_INIT_CANVAS,
    FJ_WINDOW_INIT_INPUT,
    FJ_WINDOW_INIT_LAYOUT,
    FJ_WINDOW_DEINIT,

    FJ_WINDOW_UPDATE,
    FJ_WINDOW_HINT_UPDATE,
};


FJ_DECLARE_ABSTRACT_OBJECT(fj_window_manager)
FJ_DECLARE_ABSTRACT_OBJECT(fj_window)

struct fj_window_info {
    struct fj_density2d density;
    struct fj_size2d size;
    fj_orientation_type_t orientation;
};


#endif
