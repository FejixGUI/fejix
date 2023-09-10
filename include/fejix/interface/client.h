#ifndef FEJIX_CLIENT_H_
#define FEJIX_CLIENT_H_


#include <fejix/core/base.h>


#define FJ_WAIT_FOREVER -1


struct fj_client;

struct fj_client_run_info {
    /* If >0, the client waits until the timeout expires or new shell messages
        arrive.
        If =0, the client does not wait.
        If <0, the client waits indefinitely. */
    int32_t wait_timeout_ms;

    /* If true, `fj_client_run` exits. */
    fj_bool_t should_stop;
};

struct fj_client_listener {
    fj_err_t (*init)(
        struct fj_client * client
    );

    fj_err_t (*deinit)(
        struct fj_client * client
    );

    /* Called on every event loop iteration. */
    fj_err_t (*run)(
        struct fj_client * client,
        struct fj_client_run_info * run_info
    );
};


struct fj_client * fj_client_new(void);

void fj_client_del(struct fj_client * client);

fj_err_t fj_client_run(struct fj_client * client, fj_idstring_t client_id);

/* Always returns a valid pointer. */
fj_ptr_t * fj_client_get_user_data(struct fj_client * client);

/* Always returns a valid pointer. */
const struct fj_client_listener ** fj_client_get_listener(
    struct fj_client * client
);

const struct fj_shell_listener ** fj_client_get_shell_listener(
    struct fj_client * client
);


#endif
