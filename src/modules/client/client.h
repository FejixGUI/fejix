#ifndef FEJIX_INTERNAL_CLIENT_H_
#define FEJIX_INTERNAL_CLIENT_H_


#include <fejix/core/base.h>


struct fj_client {
    
    fj_ptr_t user_data;

    const struct fj_client_listener * client_listener;
    
    union {
#ifdef FJ_PLATFORM_X11
        struct fj_x11_data * x11_data;
#endif
    } platform_data;

#ifdef FJ_FEATURE_UNIXPOLLER
    const struct fj_unixpoller_listener * unixpoller_listener;
    const struct fj_unixpoller * unixpoller_interface;    
    struct fj_unixpoller_data * unixpoller_data;
#endif

#ifdef FJ_FEATURE_SHELL
    const struct fj_shell_listener * shell_listener;
    const struct fj_shell * shell_interface;
#endif

};


/* The following functions are declared but may not be implemented.
    Check Fejix macros before calling them.  */

fj_err_t fj_x11_run(struct fj_client * client);



#endif
