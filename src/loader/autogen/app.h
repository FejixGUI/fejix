/* Automatically generated by tools/autogen/autogen.py, do not edit! */

#include <src/loader/loader.h>

#include <fejix/interface/app.h>

void *_fj_app_check_supported_ptr;
fj_err_t fj_app_check_supported(void)
{
    if (_fj_app_check_supported_ptr != NULL)
        return ((fj_err_t(*)(void))_fj_app_check_supported_ptr)();
    return FJ_ERR_UNSUPPORTED;
}

void *_fj_app_create_ptr;
fj_err_t fj_app_create(struct fj_app ** out_app, struct fj_app_create_info const * info)
{
    if (_fj_app_create_ptr != NULL)
        return ((fj_err_t(*)(struct fj_app ** out_app, struct fj_app_create_info const * info))_fj_app_create_ptr)(out_app, info);
    return FJ_ERR_UNSUPPORTED;
}

void *_fj_app_destroy_ptr;
fj_err_t fj_app_destroy(struct fj_app * app)
{
    if (_fj_app_destroy_ptr != NULL)
        return ((fj_err_t(*)(struct fj_app * app))_fj_app_destroy_ptr)(app);
    return FJ_ERR_UNSUPPORTED;
}

void *_fj_app_on_idle_ptr;
void fj_app_on_idle(struct fj_app * app, fj_app_on_idle_fn_t callback)
{
    if (_fj_app_on_idle_ptr != NULL)
        ((void(*)(struct fj_app * app, fj_app_on_idle_fn_t callback))_fj_app_on_idle_ptr)(app, callback);
    /* do nothing by default */;
}

void *_fj_app_on_finish_ptr;
void fj_app_on_finish(struct fj_app * app, fj_app_on_finish_fn_t callback)
{
    if (_fj_app_on_finish_ptr != NULL)
        ((void(*)(struct fj_app * app, fj_app_on_finish_fn_t callback))_fj_app_on_finish_ptr)(app, callback);
    /* do nothing by default */;
}

void *_fj_app_on_notification_ptr;
void fj_app_on_notification(struct fj_app * app, fj_app_on_notification_fn_t callback)
{
    if (_fj_app_on_notification_ptr != NULL)
        ((void(*)(struct fj_app * app, fj_app_on_notification_fn_t callback))_fj_app_on_notification_ptr)(app, callback);
    /* do nothing by default */;
}

void *_fj_app_launch_ptr;
fj_err_t fj_app_launch(struct fj_app * app)
{
    if (_fj_app_launch_ptr != NULL)
        return ((fj_err_t(*)(struct fj_app * app))_fj_app_launch_ptr)(app);
    return FJ_ERR_UNSUPPORTED;
}

void *_fj_app_wakeup_after_timeout_ptr;
void fj_app_wakeup_after_timeout(struct fj_app * app, fj_seconds_t timeout)
{
    if (_fj_app_wakeup_after_timeout_ptr != NULL)
        ((void(*)(struct fj_app * app, fj_seconds_t timeout))_fj_app_wakeup_after_timeout_ptr)(app, timeout);
    /* do nothing by default */;
}

void *_fj_app_wakeup_remotely_ptr;
fj_err_t fj_app_wakeup_remotely(struct fj_app * app)
{
    if (_fj_app_wakeup_remotely_ptr != NULL)
        return ((fj_err_t(*)(struct fj_app * app))_fj_app_wakeup_remotely_ptr)(app);
    return FJ_ERR_UNSUPPORTED;
}

void *_fj_app_set_finished_ptr;
void fj_app_set_finished(struct fj_app * app)
{
    if (_fj_app_set_finished_ptr != NULL)
        ((void(*)(struct fj_app * app))_fj_app_set_finished_ptr)(app);
    /* do nothing by default */;
}
