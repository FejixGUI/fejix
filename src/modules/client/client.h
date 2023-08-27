#ifndef FEJIX_INTERNAL_CLIENT_H_
#define FEJIX_INTERNAL_CLIENT_H_


#include <fejix/core/base.h>


struct fj_client {
    
    fj_ptr_t user_data;

    const struct fj_client_listener * client_listener;
    
    union {
        struct fj_x11_client * x11;
    } client_data;


#ifdef FJ_FEATURE_UNIXPOLLER
    const struct fj_unixpoller_listener * unixpoller_listener;
    fj_ptr_t unixpoller_data;
#endif

};


/* The following functions are declared but may not be implemented.
    Check Fejix macros before calling them.  */

fj_err_t fj_x11_run(struct fj_client * client);



#endif
