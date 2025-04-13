#ifndef FJ_CLOCK_H_INCLUDED
#define FJ_CLOCK_H_INCLUDED


#include <fejix/modules/app.h>


enum fj_clock_event_type {
    FJ_CLOCK_EVENT_TICK = 0,

    FJ_CLOCK_EVENT_ENUM32 = INT32_MAX,
};

FJ_OPAQUE_STRUCT_WITH_USERDATA(fj_clock_manager)
FJ_OPAQUE_STRUCT_WITH_USERDATA(fj_clock)

typedef fj_err (*fj_clock_event_callback)(
    struct fj_clock_manager *clock_manager,
    struct fj_clock *clock,
    enum fj_clock_event_type event_type,
    void *opt_event_data);

FJ_METHOD_NONNULL(
    fj_clock_create_manager,
    fj_err,
    struct fj_app_manager *app_manager,
    fj_clock_event_callback callback,
    struct fj_clock_manager **out_manager)

FJ_METHOD(fj_clock_destroy_manager, fj_err, struct fj_clock_manager *manager)

FJ_METHOD(
    fj_clock_create,
    fj_err,
    struct fj_clock_manager *manager,
    fj_time interval,
    fj_time requested_precision,
    struct fj_clock **out_clock)

FJ_METHOD(fj_clock_destroy, fj_err, struct fj_clock_manager *manager, struct fj_clock *clock)

FJ_METHOD(
    fj_clock_get_remaining_time,
    fj_err,
    struct fj_clock_manager *manager,
    struct fj_clock *clock,
    fj_time *out_remaining_time)

FJ_METHOD_LIST_BEGIN(fj_clock)
FJ_METHOD_LIST_ITEM(fj_clock_create_manager)
FJ_METHOD_LIST_ITEM(fj_clock_destroy_manager)
FJ_METHOD_LIST_ITEM(fj_clock_create)
FJ_METHOD_LIST_ITEM(fj_clock_destroy)
FJ_METHOD_LIST_ITEM(fj_clock_start_ticking)
FJ_METHOD_LIST_ITEM(fj_clock_stop_ticking)
FJ_METHOD_LIST_ITEM(fj_clock_get_remaining_time)
FJ_METHOD_LIST_END()

#endif
