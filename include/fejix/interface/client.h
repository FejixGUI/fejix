#ifndef FEJIX_INTERFACE_CLIENT_H_
#define FEJIX_INTERFACE_CLIENT_H_


#include <fejix/core/base.h>


struct fj_client FJ_PUBLICLY({ union fj_tag tag; });


struct fj_client_info {
    union fj_tag tag;

    /** Implementation-dependent extra creation information. Set to NULL if unused. */
    void *extra_info;

    /** String that should uniquely identify the app to the shell. */
    char const *name;
};


FJ_EXTERN_C_BEGIN


struct fj_client_iface {
    /**
    Callbacks and info are deep-copied where applicable.

    :param client: Returns the client or NULL on failure.
    */
    fj_err_t (*create)(struct fj_client **client, struct fj_client_info const *info);

    fj_err_t (*destroy)(struct fj_client *client);

    /**
    Runs a message polling loop.
    At the startup and at the end of each polling iteration calls ``idle``.
    */
    fj_err_t (*run)(struct fj_client *client);
};


FJ_EXTERN_C_END

#endif
