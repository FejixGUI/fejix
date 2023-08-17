#ifndef FEJIX_CLIENT_H_
#define FEJIX_CLIENT_H_


#include <fejix/core/object.h>


struct fj_client_interface {
    uint8_t _nothing;
};

struct fj_client_listener {
    fj_err_t (*set_interface)(
        struct fj_object * client,
        struct fj_client_interface * client_interface
    );

    fj_err_t (*release_interface)(
        struct fj_object * client
    );

    fj_err_t (*setup)(
        struct fj_object * client
    );

    fj_err_t (*shutdown)(
        struct fj_object * client
    );

    fj_err_t (*run)(
        struct fj_object * client
    );
};


fj_err_t fj_client_run(struct fj_object * client);


#endif
