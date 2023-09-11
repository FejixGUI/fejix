#ifndef FEJIX_INTERNAL_CLIENT_H_
#define FEJIX_INTERNAL_CLIENT_H_


#include <fejix/client.h>


#define FJ_SCHEDULE_NONE (0)
#define FJ_SCHEDULE_TIMEOUT(MILLISECONDS) (MILLISECONDS)
#define FJ_SCHEDULE_TIMEOUT_MIN (1)
#define FJ_SCHEDULE_TIMEOUT_MAX (0xEFFFFFFF)
#define FJ_SCHEDULE_IDLE (-1)
#define FJ_SCHEDULE_EXIT (-2)


struct fj_client {

    fj_idstring_t client_id;

    /* Timeout in milliseconds or magic values. */
    int32_t schedule;
    
    fj_ptr_t user_data;

/* The following structure types may not be implemented.
    Check Fejix macros and include the respective headers before using them. */
    union {
        struct fj_x11_data * x11;
        struct fj_wayland_data * wayland;
        struct fj_winapi_data * winapi;
    } platform_data;

    const struct fj_client_listener * client_listener;

#   ifdef FJ_FEATURE_SHELL
        const struct fj_shell_listener * shell_listener;
#   endif
#   ifdef FJ_FEATURE_WM
        const struct fj_wm_listener * wm_listener;
#   endif

};


/* The following functions are declared but may not be implemented.
    Check Fejix macros before calling them. */

fj_err_t fj_x11_client_run(struct fj_client * client);
fj_err_t fj_wayland_client_run(struct fj_client * client);
fj_err_t fj_winapi_client_run(struct fj_client * client);


/* This macro should be manually defined before including this header. */
#ifdef DEFINE_PLATFORM_RUNNERS

    static const struct fj_platform_runner {
        fj_utf8string_t name;
        fj_err_t (*run)(struct fj_client *);
    } platform_runners[] = {

#       ifdef FJ_PLATFORM_X11
            { "x11", fj_x11_client_run },
#       endif
#       ifdef FJ_PLATFORM_WAYLAND
            { "wayland", fj_wayland_client_run },
#       endif
#       ifdef FJ_PLATFORM_WINAPI
            { "winapi", fj_winapi_client_run },
#       endif

    };

#endif



#endif
