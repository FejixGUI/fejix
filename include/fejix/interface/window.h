#ifndef FEJIX_INTERFACE_WINDOW_H_
#define FEJIX_INTERFACE_WINDOW_H_


#include <fejix/interface/client.h>

#include <fejix/core/geometry.h>


enum fj_window_manager_request_id {
    FJ_WINDOW_MANAGER_GET_WINDOW = FJ_REQUEST_PUBLIC_STATIC_SUBINTERFACING,

    FJ_WINDOW_MANAGER_ALLOC = FJ_REQUEST_PUBLIC_INITIALISATION,
    FJ_WINDOW_MANAGER_DEALLOC,
    FJ_WINDOW_MANAGER_INIT,
    FJ_WINDOW_MANAGER_DEINIT,

    FJ_WINDOW_MANAGER_UPDATE_WINDOWS = FJ_REQUEST_PUBLIC,
};

enum fj_window_request_id {
    FJ_WINDOW_ALLOC = FJ_REQUEST_PUBLIC_INITIALISATION,
    FJ_WINDOW_DEALLOC,
    FJ_WINDOW_INIT,
    FJ_WINDOW_INIT_CANVAS,
    FJ_WINDOW_INIT_INPUT,
    FJ_WINDOW_INIT_LAYOUT,
    FJ_WINDOW_DEINIT,

    FJ_WINDOW_UPDATE = FJ_REQUEST_PUBLIC,
    FJ_WINDOW_HINT_UPDATE,
};


FJ_DECLARE_ABSTRACT_OBJECT(fj_window_manager)
FJ_DECLARE_ABSTRACT_OBJECT(fj_window)

struct fj_window_manager_init_info {
    struct fj_client *client;
};

struct fj_window_init_info {
    struct fj_window_manager *window_manager;
};

struct fj_window_info {
    struct fj_density2d density;
    struct fj_size2d size;
    fj_orientation_type_t orientation;
};


#endif
