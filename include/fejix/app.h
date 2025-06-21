/**
    \file
*/

#ifndef FEJIX_APP_H_
#define FEJIX_APP_H_


#include <fejix/base.h>


FJ_HANDLE(fj_app)

FJ_API(fj_app_new, struct fj_app **out_app)

FJ_API(fj_app_del, struct fj_app *app)

FJ_API(fj_app_run_and_exit, struct fj_app *app)

FJ_API(fj_app_request_exit, struct fj_app *app)

/** This is can be called from other thread provided that the app is not being destroyed. */
FJ_API(fj_app_request_continue, struct fj_app *app)

FJ_API(fj_app_continue_event, struct fj_app *app)

FJ_API(fj_app_exit_event, struct fj_app *app)

FJ_API(fj_app_pause_event, struct fj_app *app)

FJ_API(fj_app_suspend_event, struct fj_app *app)

FJ_API(fj_app_resume_event, struct fj_app *app)


#endif
