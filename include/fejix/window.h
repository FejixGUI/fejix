#ifndef FEJIX_WINDOW_H_
#define FEJIX_WINDOW_H_


#include <fejix/app.h>


FJ_HANDLE(fj_window)


FJ_API(fj_window_init, struct fj_app *app)

FJ_API(fj_window_deinit, struct fj_app *app)

FJ_API(fj_window_new, struct fj_app *app, struct fj_window **out_window)

FJ_API(fj_window_del, struct fj_app *app, struct fj_window *window)

FJ_API(fj_window_sync, struct fj_app *app, struct fj_window *window)

FJ_API(
    fj_window_set_preferred_size,
    struct fj_app *app,
    struct fj_window *window,
    struct fj_size2d const *size)

FJ_API(fj_window_set_title, struct fj_app *app, struct fj_window *window, char const *title)

FJ_API(fj_window_request_update, struct fj_app *app, struct fj_window *window)

FJ_API_VOID(fj_window_update_event, struct fj_app *app, struct fj_window *window)

FJ_API_VOID(fj_window_close_event, struct fj_app *app, struct fj_window *window)

FJ_API_VOID(fj_window_size_event, struct fj_app *app, struct fj_window *window)

FJ_API_VOID(fj_window_orientation_event, struct fj_app *app, struct fj_window *window)

FJ_API_VOID(fj_window_density_event, struct fj_app *app, struct fj_window *window)


#endif
