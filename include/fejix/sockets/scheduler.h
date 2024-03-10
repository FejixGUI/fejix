#ifndef FEJIX_SOCKET_SCHEDULER_H_
#define FEJIX_SOCKET_SCHEDULER_H_


#include <fejix/bus.h>


#define FJ_SCHEDULER_TIMEOUT_NONE (0)
#define FJ_SCHEDULER_TIMEOUT_INFINITE (UINT32_MAX)


enum fj_scheduler_message_id {
    /** Sent to the user between processing of other messages. */
    FJ_SCHEDULER_CONTINUE = FJ_FIRST_MESSAGE_ID(FJ_SOCKET_SCHEDULER),

    /** Sets the preferred time after which the next FJ_SCHEDULE_CONTINUE
        message should be generated.
        Message processing may interrupt the schedule, so the timeout may not
        be precise.

        Default schedule is infinite timeout. */
    FJ_SCHEDULER_SCHEDULE,
};


struct fj_scheduler_schedule {
    uint32_t timeout_ms;
};


#endif
