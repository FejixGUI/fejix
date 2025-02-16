/* Automatically generated by tools/autogen.py, do not edit! */

#include <src/loader/loader.h>

#include <fejix/interface/app_clock.h>

fj_loader_function_t _fj_has_app_clock_ptr;
void fj_has_app_clock(void)
{
    if (_fj_has_app_clock_ptr != NULL) {
        ((void(*)(void))_fj_has_app_clock_ptr)();
    }
    /* do nothing by default */;
}

fj_loader_function_t _fj_app_clock_create_ptr;
fj_err_t fj_app_clock_create(struct fj_app * app, struct fj_app_clock ** out_clock, void * clock_userdata)
{
    if (_fj_app_clock_create_ptr != NULL) {
        return ((fj_err_t(*)(struct fj_app * app, struct fj_app_clock ** out_clock, void * clock_userdata))_fj_app_clock_create_ptr)(app, out_clock, clock_userdata);
    }
    return FJ_ERR_UNIMPLEMENTED;
}

fj_loader_function_t _fj_app_clock_destroy_ptr;
fj_err_t fj_app_clock_destroy(struct fj_app * app, struct fj_app_clock * clock)
{
    if (_fj_app_clock_destroy_ptr != NULL) {
        return ((fj_err_t(*)(struct fj_app * app, struct fj_app_clock * clock))_fj_app_clock_destroy_ptr)(app, clock);
    }
    return FJ_ERR_UNIMPLEMENTED;
}

fj_loader_function_t _fj_app_clock_start_ticking_ptr;
fj_err_t fj_app_clock_start_ticking(struct fj_app * app, struct fj_app_clock * clock, fj_microseconds_t tick_interval, fj_app_clock_resolution_t flags)
{
    if (_fj_app_clock_start_ticking_ptr != NULL) {
        return ((fj_err_t(*)(struct fj_app * app, struct fj_app_clock * clock, fj_microseconds_t tick_interval, fj_app_clock_resolution_t flags))_fj_app_clock_start_ticking_ptr)(app, clock, tick_interval, flags);
    }
    return FJ_ERR_UNIMPLEMENTED;
}

fj_loader_function_t _fj_app_clock_stop_ticking_ptr;
fj_err_t fj_app_clock_stop_ticking(struct fj_app * app, struct fj_app_clock * clock)
{
    if (_fj_app_clock_stop_ticking_ptr != NULL) {
        return ((fj_err_t(*)(struct fj_app * app, struct fj_app_clock * clock))_fj_app_clock_stop_ticking_ptr)(app, clock);
    }
    return FJ_ERR_UNIMPLEMENTED;
}

fj_loader_function_t _fj_app_clock_on_tick_ptr;
void fj_app_clock_on_tick(struct fj_app * app, fj_app_clock_on_tick_fn_t callback)
{
    if (_fj_app_clock_on_tick_ptr != NULL) {
        ((void(*)(struct fj_app * app, fj_app_clock_on_tick_fn_t callback))_fj_app_clock_on_tick_ptr)(app, callback);
    }
    /* do nothing by default */;
}

fj_loader_function_t _fj_app_clock_get_remaining_time_ptr;
fj_microseconds_t fj_app_clock_get_remaining_time(struct fj_app * app, struct fj_app_clock * clock)
{
    if (_fj_app_clock_get_remaining_time_ptr != NULL) {
        return ((fj_microseconds_t(*)(struct fj_app * app, struct fj_app_clock * clock))_fj_app_clock_get_remaining_time_ptr)(app, clock);
    }
    return 0;
}

