#ifndef FEJIX_WINDOW_H_
#define FEJIX_WINDOW_H_


#include <fejix/app.h>


FJ_HANDLE(fj_window)


FJ_API(fj_window_init, struct fj_app *app)

FJ_API(fj_window_deinit, struct fj_app *app)

FJ_API(fj_window_new, struct fj_app *app, struct fj_window **out_window)

FJ_API(fj_window_del, struct fj_app *app, struct fj_window *window)

FJ_API(fj_window_update, struct fj_app *app, struct fj_window *window)

FJ_API_VOID(fj_window_update_event, struct fj_app *app, struct fj_window *window)

FJ_API_VOID(fj_window_close_event, struct fj_app *app, struct fj_window *window)


#endif
