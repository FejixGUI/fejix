#ifndef FEJIX_INTERFACE_CLIENT_H_
#define FEJIX_INTERFACE_CLIENT_H_


#include <fejix/core/base.h>


struct fj_client FJ_PUBLICLY({ union fj_tag tag; });


struct fj_client_callbacks {
    fj_err_t (*idle)(struct fj_client *client);
};

struct fj_client_create_info {
    union fj_tag tag;

    /** Implementation-dependent extra creation information. Set to NULL if unused. */
    void *extra_info;

    /** String that should uniquely identify the app to the shell. */
    char const *name;
};

struct fj_client_funcs {
    /**
    Callbacks and info are deep-copied where applicable.

    :param client: Returns the client or NULL on failure.
    */
    fj_err_t (*create)(
        struct fj_client **client,
        struct fj_client_callbacks const *callbacks,
        struct fj_client_create_info const *info
    );

    fj_err_t (*destroy)(struct fj_client *client);

    /**
    Runs a message polling loop.
    At the startup and at the end of each polling iteration calls ``idle``.
    */
    fj_err_t (*run)(struct fj_client *client);

    /** Thread-safe provided that the client is not being destructed. */
    fj_err_t (*wakeup)(struct fj_client *client);

    void (*request_quit)(struct fj_client *client);

    void (*request_idle)(struct fj_client *client);
};

#endif
