#ifndef FEJIX_WINAPI_SCHEDULER_H_
#define FEJIX_WINAPI_SCHEDULER_H_


#include <fejix/interface/scheduler.h>


struct fj_scheduler_common {
    struct fj_scheduler_callbacks callbacks;
    struct fj_schedule schedule;
    fj_timeout_t timeout_resolution;
};


fj_bool8_t fj_winapi_scheduler_needs_quit(struct fj_client *client);

fj_err_t fj_winapi_scheduler_schedule_sleep(struct fj_client *client);

void fj_winapi_scheduler_schedule_quit(struct fj_client *client);

fj_err_t fj_winapi_scheduler_sleep(struct fj_client *client);


#endif
