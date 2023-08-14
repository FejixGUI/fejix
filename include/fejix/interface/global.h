#ifndef FEJIX_GLOBAL_H_
#define FEJIX_GLOBAL_H_


#include <fejix/core/base.h>
#include <fejix/core/map.h>


enum fj_registry {
    FJ_PUBLIC_ID_SPACE_BEGIN = 0,

    FJ_CLIENT_LISTENER,
    FJ_WINDOW_MANAGER_LISTENER,

    FJ_WINDOW_LISTENER,
    FJ_WINDOW_CONFIG_LISTENER,

    FJ_PUBLIC_ID_SPACE_END = 999,

    FJ_PRIVATE_ID_SPACE_BEGIN = 1000,
    
    FJ_PRIVATE_ID_SPACE_END = 9999,

    FJ_USER_ID_SPACE_BEGIN = 10000,
};


fj_err_t fj_global_run(struct fj_map * global_object);


#endif
