#ifndef FEJIX_WINAPI_TIMER_H_
#define FEJIX_WINAPI_TIMER_H_

#include <fejix/modules/io_thread_timer.h>
#include <fejix/utils/list.h>

#include <windows.h>


/** All fields are in performace counter ticks. */
struct fj_io_thread_timer {
    LARGE_INTEGER start;
    LARGE_INTEGER period;
};


FJ_VECTOR_OF(fj_winapi_timer_vector, struct fj_io_thread_timer *)


struct fj_io_thread_timer_manager {
    void *userdata;
    struct fj_winapi_timer_vector timers;
    fj_io_thread_timer_event_callback callback;
};


/** \returns milliseconds, INFINITE if there are no timers. */
DWORD fj_winapi_timer_manager_get_sleep_duration(struct fj_io_thread_timer_manager *manager);


#endif
