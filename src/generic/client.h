#ifndef FEJIX_GENERIC_CLIENT_H_
#define FEJIX_GENERIC_CLIENT_H_


#include <fejix/client.h>


struct fj_client {

    fj_idstring_t client_id;

    fj_schedule_t schedule;

    fj_ptr_t user_data;

    const struct fj_client_listener * client_listener;

    /* The following structure types may not be implemented. Check Fejix macros
        and include the respective headers before using them. */
    union {
        struct fj_x11_data * x11;
        struct fj_wayland_data * wayland;
        struct fj_winapi_data * winapi;
    } data;

#ifdef FJ_FEATURE_SHELL
    const struct fj_shell_listener * shell_listener;
#endif
#ifdef FJ_FEATURE_WM
    const struct fj_wm_listener * wm_listener;
#endif

};

#ifdef FJ_PLATFORM_X11
    fj_err_t fj_x11_client_run(struct fj_client * client);
#endif
#ifdef FJ_PLATFORM_WAYLAND
    fj_err_t fj_wayland_client_run(struct fj_client * client);
#endif
#ifdef FJ_PLATFORM_WINAPI
    fj_err_t fj_winapi_client_run(struct fj_client * client);
#endif


/* This macro should be manually defined before including this header. */
#ifdef FJ_INTERNAL_CLIENT_IMPLEMENTATION

    /* Order of the platforms is NOT important here. */
    static const struct fj_platform_runner {
        fj_utf8string_t name;
        fj_err_t (*run)(struct fj_client *);
    } platform_runners[] = {

#ifdef FJ_PLATFORM_WINAPI
        { "winapi", fj_winapi_client_run },
#endif
#ifdef FJ_PLATFORM_WAYLAND
        { "wayland", fj_wayland_client_run },
#endif
#ifdef FJ_PLATFORM_X11
        { "x11", fj_x11_client_run },
#endif

    };

    /* Order of the platforms IS important here.
        The first available platform will be default.
        The default platform is loaded when ENV gives no platform hints. */
    static const fj_utf8string_t fj_platform_names[] = {

#ifdef FJ_PLATFORM_WINAPI
        "winapi",
#endif
#ifdef FJ_PLATFORM_WAYLAND
        "wayland",
#endif
#ifdef FJ_PLATFORM_X11
        "x11",
#endif

    };

#endif



#endif
