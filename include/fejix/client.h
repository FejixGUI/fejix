#ifndef FEJIX_CLIENT_H_
#define FEJIX_CLIENT_H_


#include <fejix/base.h>


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


struct fj_client_info {
    fj_string_t client_id;

    fj_err_t (*init)
};


struct fj_platform_entrypoint {
    fj_string_t platform_name;

    fj_err_t (*launch)(
        void * user_data,
        struct fj_client_info * client_info
    );
};


fj_string_t fj_get_selected_platform_name(void);




#endif
