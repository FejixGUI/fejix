/**
    \file
*/

#ifndef FEJIX_APP_H_
#define FEJIX_APP_H_


#include <fejix/base.h>


FJ_APP(fj_app)

FJ_API(fj_app_new, struct fj_app **out_app, void *extra_info)

FJ_API(fj_app_del, struct fj_app *app)

FJ_API(fj_app_run, struct fj_app *app)

FJ_API(fj_app_quit, struct fj_app *app)

/**
    Wakes up an application that is waiting for events.
    This asks the system to send a custom event that gets handled with fj_app_ping_callback().

    This can be called from another thread provided that the app is not being destroyed.

    This is not supposed to be called periodically from the main thread as this goes through an
    inefficient process of communicating with the system.
    To regularly invoke a callback use a timer with zero timeout period.
*/
FJ_API(fj_app_ping, struct fj_app *app)

FJ_API_VOID(fj_app_ping_callback, struct fj_app *app)

FJ_API_VOID(fj_app_start_callback, struct fj_app *app)

FJ_API_VOID(fj_app_quit_callback, struct fj_app *app)

FJ_API_VOID(fj_app_pause_callback, struct fj_app *app)

FJ_API_VOID(fj_app_resume_callback, struct fj_app *app)

FJ_API_VOID(fj_app_unload_callback, struct fj_app *app)

FJ_API_VOID(fj_app_load_callback, struct fj_app *app)


#endif
