#ifndef FEJIX_INTERNAL_CLIENT_H_
#define FEJIX_INTERNAL_CLIENT_H_


#include <fejix/core/base.h>


struct fj_client {

    fj_idstring_t client_id;
    
    fj_ptr_t user_data;

    union {
#ifdef FJ_PLATFORM_X11
        struct fj_x11_data * x11_data;
#endif
    } platform_data;

    const struct fj_client_listener * client_listener;

#ifdef FJ_FEATURE_SHELL
    const struct fj_shell_listener * shell_listener;
#endif

};


/* The following functions are declared but may not be implemented.
    Check Fejix macros before calling them.  */

fj_err_t fj_x11_client_run(struct fj_client * client);



#endif
