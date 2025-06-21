#include "app.h"

#include <src/shared/utils/memory.h>

#include <X11/Xlib-xcb.h>


static enum fj_error fj_app_new_(struct fj_app **out_app, void *extra_info)
{
    (void) extra_info;

    enum fj_error e;

    e = FJ_ALLOC(out_app);

    if (e)
        return e;

    e = fj_unix_events_init(&(*out_app)->events, *out_app);

    if (e) {
        FJ_FREE(out_app);
        return e;
    }

    (*out_app)->display = XOpenDisplay(NULL);

    if ((*out_app)->display == NULL) {
        fj_unix_events_deinit(&(*out_app)->events);
        FJ_FREE(out_app);
        return FJ_ERROR_OPERATION_FAILED;
    }

    (*out_app)->connection = XGetXCBConnection((*out_app)->display);
    XSetEventQueueOwner((*out_app)->display, XCBOwnsEventQueue);

    return FJ_OK;
}


static enum fj_error fj_app_del_(struct fj_app *app)
{
    fj_unix_events_deinit(&app->events);
    FJ_FREE(&app);

    if (XCloseDisplay(app->display) != 0) {
        return FJ_ERROR_OPERATION_FAILED;
    }

    return FJ_OK;
}


static enum fj_error fj_app_run_and_exit_(struct fj_app *app)
{
    enum fj_error e;

    for (;;) {
        fj_app_continue_event(app);

        if (app->exit_requested)
            break;

        e = fj_unix_events_wait(&app->events, NULL);
        if (e) {
            fj_app_exit_event(app);
            return e;
        }
    }

    fj_app_exit_event(app);
    return FJ_OK;
}


enum fj_error fj_app_request_exit_(struct fj_app *app)
{
    app->exit_requested = true;
    return FJ_OK;
}


enum fj_error fj_app_request_continue_(struct fj_app *app)
{
    return fj_unix_events_echo(&app->events);
}


void fj_x11_init_app(void)
{
    FJ_API_INIT(fj_app_new)
    FJ_API_INIT(fj_app_del)
    FJ_API_INIT(fj_app_run_and_exit)
    FJ_API_INIT(fj_app_request_exit)
    FJ_API_INIT(fj_app_request_continue)
}
