#ifndef FEJIX_WINAPI_SLEEP_TIMER_H_
#define FEJIX_WINAPI_SLEEP_TIMER_H_


#include <fejix/interface/sleep_timer.h>


struct fj_sleep_timer_manager {
    /** 0 means unset. */
    fj_timeout_t timeout;
};


fj_timeout_t fj_winapi_sleep_timer_get_timeout(struct fj_client *client);


#endif
