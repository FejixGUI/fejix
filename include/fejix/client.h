#ifndef FEJIX_CLIENT_H_
#define FEJIX_CLIENT_H_


#include <fejix/core/base.h>


/* On each event loop iteration the client processes available shell
    messages and then calls the user callback.
    The callback gets called even if no messages were processed.
    This is the default behavior. */
#define FJ_SCHEDULE_NONE (0)

/* On each event looop iteration the client waits indefinitely until new
    messages arrive and then calls the user callback.
    The callback gets called only when new messages arrive. */
#define FJ_SCHEDULE_IDLE (-1)

/* Event loop ends as soon as the current iteration ends. */
#define FJ_SCHEDULE_EXIT (-2)

/* On each event looop iteration the client waits until either the timeout
    expires or new messages arrive and then calls the user callback.
    The callback gets called only when new messages arrive. */
#define FJ_SCHEDULE_TIMEOUT(MILLISECONDS) (MILLISECONDS)

/* Minimal timeout is 1 millisecond. */
#define FJ_SCHEDULE_TIMEOUT_MIN (1)

/* Maximal timeout is 2147483647 milliseconds or approximately 25 days. */
#define FJ_SCHEDULE_TIMEOUT_MAX (0x7FFFFFFF)

#define FJ_SCHEDULE_IS_TIMEOUT(SCHEDULE) \
    ((SCHEDULE) >= FJ_SCHEUDLE_TIMEOUT_MIN \
    && (SCHEDULE) <= FJ_SCHEDULE_TIMEOUT_MAX)


/* Contains timeout in milliseconds or magic values. */
typedef int32_t fj_schedule_t;


struct fj_client;

FJ_DEFINE_VERSION(fj_client_listener, v_0_1)
struct fj_client_listener {
    fj_err_t (*init)(struct fj_client * client);

    fj_err_t (*deinit)(struct fj_client * client);

    /* Called on every event loop iteration. */
    fj_err_t (*run)(struct fj_client * client);
};


struct fj_client * fj_client_new(fj_idstring_t client_id);

void fj_client_del(struct fj_client * client);

/* Returns the name of the platform derived from the defined env variables. */
fj_utf8string_t fj_get_selected_platform(void);

/* Get a list of all built-in platforms. */
void fj_client_get_platforms(uint32_t * count, fj_utf8string_t const ** names);

/* Runs the platform selected by the `fj_client_get_selected_platform`.
    If client does not have the client listener set, returns an error. */
fj_err_t fj_client_run(struct fj_client * client);

void fj_client_set_schedule(struct fj_client * client, fj_schedule_t schedule);

fj_schedule_t fj_client_get_schedule(struct fj_client * client);

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
