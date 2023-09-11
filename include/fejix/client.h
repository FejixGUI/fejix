#ifndef FEJIX_CLIENT_H_
#define FEJIX_CLIENT_H_


#include <fejix/core/base.h>


struct fj_client;

struct fj_client_listener {
    fj_err_t (*init)(struct fj_client * client);

    fj_err_t (*deinit)(struct fj_client * client);

    /* Called on every event loop iteration. */
    fj_err_t (*run)(struct fj_client * client);
};


struct fj_client * fj_client_new(fj_idstring_t client_id);

void fj_client_del(struct fj_client * client);

fj_err_t fj_client_run(struct fj_client * client);

/* Default behavior. The client only processes available shell messages and
    calls `run` repeatedly. */
void fj_client_schedule_none(struct fj_client * client);

/* The client waits until either the timeout expires or new messages arrive. */
void fj_client_schedule_timeout(
    struct fj_client * client,
    uint32_t milliseconds
);

/* The client waits indefinitely until new messages arrive. */
void fj_client_schedule_idle(struct fj_client * client);

/* The `fj_client_run` function exits. */
void fj_client_schedule_exit(struct fj_client * client);

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
