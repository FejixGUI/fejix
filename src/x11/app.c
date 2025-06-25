#include "app.h"

#include <src/shared/utils/logging.h>
#include <src/shared/utils/memory.h>

#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib-xcb.h>


#define EVENT_MASK (~0x80)  // Just a weirdness of XCB


static char const *const atom_names[FJ_X11_ATOM_MAX] = {
    [FJ_X11_ATOM_NET_WM_NAME] = "_NET_WM_NAME",
    [FJ_X11_ATOM_UTF8_STRING] = "UTF8_STRING",
};

static int xlib_last_error = Success;

static int xlib_error_handler(Display *display, XErrorEvent *error)
{
    (void) display;
    xlib_last_error = error->error_code;
    return 0;
}

int fj_x11_xlib_get_last_error(void)
{
    return xlib_last_error;
}

void fj_x11_xlib_clear_last_error(void)
{
    xlib_last_error = Success;
}


static enum fj_error process_event(struct fj_app *app, xcb_generic_event_t *event)
{
    switch (event->response_type & EVENT_MASK) {
        default:
            break;
    }

    return FJ_OK;
}


static enum fj_error process_events(void *callback_data, int file_descriptor, short event_mask)
{
    (void) file_descriptor;
    enum fj_error e;

    if (event_mask & (POLLERR | POLLHUP | POLLNVAL)) {
        FJ_ERROR("got errors while reading events from the X11 display");
        return FJ_ERROR_IO_FAILED;
    }

    struct fj_app *app = callback_data;

    for (;;) {
        xcb_generic_event_t *event = xcb_poll_for_event(app->connection);

        if (event == NULL)
            break;

        e = process_event(app, event);
        free(event);
        if (e)
            return e;  // FIXME What is the best way to handle errors in a loop?
        /** We can alternatively ignore errors unless the user quits the app. */
    }

    return FJ_OK;
}


static void deinit_events(struct fj_app *app)
{
    fj_unix_events_deinit(&app->events);
}

static enum fj_error get_atoms(struct fj_app *app)
{
    xcb_intern_atom_cookie_t coockies[FJ_X11_ATOM_MAX];

    for (enum fj_x11_atom atom = 0; atom < FJ_X11_ATOM_MAX; atom++) {
        coockies[atom]
            = xcb_intern_atom(app->connection, 1, strlen(atom_names[atom]), atom_names[atom]);
    }

    xcb_flush(app->connection);

    for (enum fj_x11_atom atom = 0; atom < FJ_X11_ATOM_MAX; atom++) {
        xcb_intern_atom_reply_t *reply
            = xcb_intern_atom_reply(app->connection, coockies[atom], NULL);

        app->atoms[atom] = reply->atom;

        free(reply);
    }

    return FJ_OK;
}


static enum fj_error fj_app_del_(struct fj_app *app)
{
    deinit_events(app);

    if (app->display != NULL) {
        int result = XCloseDisplay(app->display);
        if (result != 0)
            return FJ_ERROR_OPERATION_FAILED;
    }

    FJ_FREE(&app);

    return FJ_OK;
}


static enum fj_error fj_app_new_(struct fj_app **out_app, void *extra_info)
{
    (void) extra_info;
    enum fj_error e;

    struct fj_app *app;
    e = FJ_ALLOC(&app);
    if (e)
        return e;

    app->display = XOpenDisplay(NULL);
    if (app->display == NULL) {
        FJ_ERROR("cannot open X11 display");
        fj_app_del_(app);
        return FJ_ERROR_OPERATION_FAILED;
    }

    app->connection = XGetXCBConnection(app->display);
    XSetEventQueueOwner(app->display, XCBOwnsEventQueue);
    XSetErrorHandler(xlib_error_handler);
    get_atoms(app);

    e = fj_unix_events_init(&app->events, app);
    if (e) {
        fj_app_del_(app);
        return e;
    }

    e = fj_unix_events_add(
        &app->events, xcb_get_file_descriptor(app->connection), POLLIN, process_events);
    if (e) {
        fj_app_del_(app);
        return e;
    }

    *out_app = app;
    return FJ_OK;
}


static enum fj_error fj_app_run_(struct fj_app *app)
{
    enum fj_error e;

    for (;;) {
        // TODO do timers

        if (app->should_quit)
            break;

        e = fj_unix_events_wait(&app->events, NULL);
        if (e) {
            fj_app_quit_callback(app);
            return e;
        }
    }

    fj_app_quit_callback(app);
    return FJ_OK;
}


enum fj_error fj_app_quit_(struct fj_app *app)
{
    app->should_quit = true;
    return FJ_OK;
}


enum fj_error fj_app_ping_(struct fj_app *app)
{
    return fj_unix_events_ping(&app->events);
}


void fj_x11_init_app(void)
{
    FJ_API_INIT(fj_app_new)
    FJ_API_INIT(fj_app_del)
    FJ_API_INIT(fj_app_run)
    FJ_API_INIT(fj_app_quit)
    FJ_API_INIT(fj_app_ping)
}
