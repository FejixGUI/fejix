#ifndef FEJIX_APP_H_
#define FEJIX_APP_H_

#include <fejix/base.h>


struct fj_app;


enum fj_app_run_modes
{
    FJ_APP_RUN_MODE_RUNNABLE = 1 << 0,
    FJ_APP_RUN_MODE_QUITTABLE = 1 << 1,
    FJ_APP_RUN_MODE_ITERABLE = 1 << 2,
    FJ_APP_RUN_MODE_AWAITABLE = 1 << 3,
    FJ_APP_RUN_MODE_ENUM_MAX = INT32_MAX,
};

FJ_API(fj_app_create, struct fj_app **out_app, void *user_data)

FJ_API(fj_app_destroy, struct fj_app *app)

FJ_API(
    fj_app_on_set_run_modes,
    struct fj_app *app,
    enum fj_app_run_modes const modes)

FJ_API(fj_app_run, struct fj_app *app)

FJ_API(fj_app_iterate, struct fj_app *app)

FJ_API(fj_app_await, struct fj_app *app)

FJ_API(fj_app_on_iterate, struct fj_app *app)

FJ_API(fj_app_quit, struct fj_app *app)

FJ_API(fj_app_on_quit, struct fj_app *app)

FJ_API(fj_app_ping, struct fj_app *app)

FJ_API(fj_app_on_ping, struct fj_app *app)

FJ_API(fj_app_on_activate, struct fj_app *app)

FJ_API(fj_app_on_deactivate, struct fj_app *app)

FJ_API(fj_app_on_suspend, struct fj_app *app)

FJ_API(fj_app_on_resume, struct fj_app *app)

FJ_API(fj_app_on_set_system_handle, struct fj_app *app, uintptr_t handle)

#endif
