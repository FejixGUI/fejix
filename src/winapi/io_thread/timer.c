#include <src/winapi/io_thread/io_thread.h>
#include <src/winapi/io_thread/timer.h>

#include <fejix/utils/math.h>
#include <fejix/utils/memory.h>
#include <fejix/utils/vector.h>

#include <limits.h>


DWORD fj_winapi_timer_manager_get_sleep_time(struct fj_io_thread_timer_manager *manager)
{
    if (manager->timers.length == 0) {
        return INFINITE;
    }

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    LONGLONG closest = LLONG_MAX;

    for (uint32_t i = 0; i < manager->timers.length; i++) {
        LONGLONG remaining = (manager->timers.items[i]->start.QuadPart - now.QuadPart)
            / manager->timers.items[i]->period.QuadPart;

        if (remaining < closest) {
            closest = remaining;
        }
    }

    return closest * 1000LL / frequency.QuadPart;
}


enum fj_error fj_io_thread_timer_create_manager_winapi(
    struct fj_io_thread *io_thread, struct fj_io_thread_timer_manager **out_manager)
{
    *out_manager = &io_thread->timer_manager;
    (*out_manager)->callback = fj_io_thread_timer_event_callback_default;
    return FJ_OK;
}


enum fj_error fj_io_thread_timer_destroy_manager_winapi(struct fj_io_thread_timer_manager *manager)
{
    (void) manager;
    return FJ_OK;
}


void fj_io_thread_timer_set_callback_winapi(
    struct fj_io_thread_timer_manager *manager, fj_io_thread_timer_event_callback callback)
{
    manager->callback = callback;
}


enum fj_error fj_io_thread_timer_create_winapi(
    struct fj_io_thread_timer_manager *manager,
    fj_time initial_delay,
    fj_time repeat_period,
    fj_time requested_precision,
    struct fj_io_thread_timer **out_timer)
{
    enum fj_error e;

    e = FJ_ALLOC_ZEROED(out_timer);

    if (e)
        return e;

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    (*out_timer)->start.QuadPart
        = now.QuadPart + frequency.QuadPart * (LONGLONG) fj_time_into_millis(initial_delay);

    (*out_timer)->period.QuadPart
        = frequency.QuadPart * (LONGLONG) fj_time_into_millis(repeat_period);

    (void) requested_precision;  // TODO precise timers based on SetTimer and timeSetEvent

    e = fj_winapi_timer_vector_push(&manager->timers, out_timer);

    if (e) {
        FJ_FREE(out_timer);
        return e;
    }

    return FJ_OK;
}


enum fj_error fj_io_thread_timer_destroy_winapi(
    struct fj_io_thread_timer_manager *manager, struct fj_io_thread_timer *timer)
{
    enum fj_error e;

    uint32_t index;
    for (index = 0; index < manager->timers.length; index++) {
        if (manager->timers.items[index] == timer) {
            break;
        }
    }

    e = fj_winapi_timer_vector_remove(&manager->timers, index);

    if (e)
        return e;

    FJ_FREE(&timer);

    return FJ_OK;
}
