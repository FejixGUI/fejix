#include "app.h"

#include <src/shared/common/error.h>
#include <src/shared/common/memory.h>

#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib-xcb.h>


#define EVENT_MASK (~0x80)  // Just a weirdness of XCB


char const *fj_x11_error_into_string(uint8_t error_code)
{
    static char const *const messages[] = {
        [Success] = "Success: everything is okay",
        [BadRequest] = "BadRequest: bad request code",
        [BadValue] = "BadValue: int parameter out of range",
        [BadWindow] = "BadWindow: parameter not a Window",
        [BadPixmap] = "BadPixmap: parameter not a Pixmap",
        [BadAtom] = "BadAtom: parameter not an Atom",
        [BadCursor] = "BadCursor: parameter not a Cursor",
        [BadFont] = "BadFont: parameter not a Font",
        [BadMatch] = "BadMatch: parameter mismatch",
        [BadDrawable] = "BadDrawable: parameter not a Pixmap or Window",
        [BadAccess] = "BadAccess: (depends on context)",
        [BadAlloc] = "BadAlloc: insufficient resources",
        [BadColor] = "BadColor: no such colormap",
        [BadGC] = "BadGC: parameter not a GC",
        [BadIDChoice] = "BadIDChoice: choice not in range or already used",
        [BadName] = "BadName: font or color name doesn't exist",
        [BadLength] = "BadLength: Request length incorrect",
        [BadImplementation] = "BadImplementation: server is defective",
    };

    if (error_code >= FJ_LEN(messages)) {
        return "non-standard error";
    }

    return messages[error_code];
}


static int xlib_error_handler(Display *display, XErrorEvent *error)
{
    (void) display;
    FJ_ERRORF("Xlib error: %s", fj_x11_error_into_string(error->error_code));
    return 0;
}

char const *fj_x11_xcb_error_into_string(xcb_generic_error_t *error)
{
    return fj_x11_error_into_string(error->error_code);
}


static fj_err process_event(struct fj_app *app, xcb_generic_event_t *event)
{
    // TODO delegate window events to some function in window.c
    switch (event->response_type & EVENT_MASK) {
        default:
            break;

            // case XCB_CLIENT_MESSAGE: {
            //     xcb_client_message_event_t *client_message = (void *) event;
            //     uint32_t *data = client_message->data.data32;
            //     uint32_t message_type = data[0];

            //     for (uint32_t i = 0; i < app->window_service.windows.length;
            //     i++) {
            //         if (app->window_service.windows.items[i]->id
            //             == client_message->window)
            //         {
            //             if (message_type == FJ_X11_GET_ATOM(app,
            //             WM_DELETE_WINDOW))
            //             {
            //                 fj_window_close_callback(
            //                     app->window_service.windows.items[i]);
            //             }
            //             break;
            //         }
            //     }
            // }
    }

    return FJ_OK;
}


static fj_err process_events(
    void *callback_data, int file_descriptor, short event_mask)
{
    (void) file_descriptor;
    fj_err e;

    if (event_mask & (POLLERR | POLLHUP | POLLNVAL)) {
        FJ_ERROR("got errors while reading events from the X11 display");
        return FJ_ERR_SYSTEM;
    }

    struct fj_app *app = callback_data;

    for (;;) {
        xcb_generic_event_t *event = xcb_poll_for_event(app->_data->connection);

        if (event == NULL)
            break;

        e = process_event(app, event);
        free(event);
        if (e)
            return e;
    }

    return FJ_OK;
}


static void deinit_events(struct fj_app *app)
{
    fj_unix_events_deinit(&app->_data->events);
}

static fj_err get_atoms(struct fj_app *app)
{
#define FJ_X11_ATOM_LIST_ITEM(X) [FJ_X11_ATOM_ID(X)] = #X,
    static char const *const atom_names[FJ_X11_ATOM_MAX] = { FJ_X11_ATOM_LIST };
#undef FJ_X11_ATOM_LIST_ITEM

    xcb_intern_atom_cookie_t coockies[FJ_X11_ATOM_MAX];

    for (enum fj_x11_atom atom = 0; atom < FJ_X11_ATOM_MAX; atom++) {
        coockies[atom] = xcb_intern_atom(
            app->_data->connection,
            1,
            strlen(atom_names[atom]),
            atom_names[atom]);
    }

    xcb_flush(app->_data->connection);

    for (enum fj_x11_atom atom = 0; atom < FJ_X11_ATOM_MAX; atom++) {
        xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(
            app->_data->connection, coockies[atom], NULL);

        app->_data->atoms[atom] = reply->atom;

        free(reply);
    }

    return FJ_OK;
}


static fj_err deinit_(struct fj_app *app)
{
    deinit_events(app);

    if (app->_data->display != NULL) {
        int result = XCloseDisplay(app->_data->display);
        if (result != 0) {
            FJ_ERRORF(
                "cannot close X11 display: %s",
                fj_x11_error_into_string(result));
        }
    }

    FJ_FREE(&app->_data);

    return FJ_OK;
}


static fj_err init_(struct fj_app *app, void *extra_init_data)
{
    (void) extra_init_data;
    fj_err e;

    e = FJ_ALLOC(&app->_data);
    if (e)
        return e;

    app->_data->display = XOpenDisplay(NULL);
    if (app->_data->display == NULL) {
        FJ_ERROR("cannot open X11 display");
        deinit_(app);
        return FJ_ERR_SYSTEM;
    }

    app->_data->connection = XGetXCBConnection(app->_data->display);
    XSetEventQueueOwner(app->_data->display, XCBOwnsEventQueue);
    XSetErrorHandler(xlib_error_handler);
    get_atoms(app);

    e = fj_unix_events_init(&app->_data->events, app);
    if (e) {
        deinit_(app);
        return e;
    }

    e = fj_unix_events_add(
        &app->_data->events,
        xcb_get_file_descriptor(app->_data->connection),
        POLLIN,
        process_events);
    if (e) {
        deinit_(app);
        return e;
    }

    return FJ_OK;
}


static fj_err loop_(struct fj_app *app)
{
    fj_err e;

    app->send(app, FJ_APP_DID_START_LOOP, NULL);

    for (;;) {
        // TODO implement timers

        if (app->_data->should_stop)
            break;

        e = fj_unix_events_wait(&app->_data->events, NULL);
        if (e) {
            app->send(app, FJ_APP_DID_STOP_LOOP, NULL);
            return e;
        }
    }

    app->send(app, FJ_APP_DID_STOP_LOOP, NULL);
    return FJ_OK;
}


static fj_err stop_loop_(struct fj_app *app)
{
    app->_data->should_stop = true;
    return FJ_OK;
}


static void get_loop_flags_(enum fj_app_loop_flags *out_flags)
{
    *out_flags = FJ_APP_LOOP_FLAG_TOPLEVEL | FJ_APP_LOOP_FLAG_REENTRANT
        | FJ_APP_LOOP_FLAG_STOPPABLE;
}


static fj_err ping_(struct fj_app *app)
{
    return fj_unix_events_ping(&app->_data->events);
}


fj_err fj_x11_app_sender(void *app, int32_t message, void *data)
{
    switch (message) {
        case FJ_APP_INIT:
            return init_(app, data);

        case FJ_APP_DEINIT:
            return deinit_(app);

        case FJ_APP_LOOP:
            return loop_(app);

        case FJ_APP_STOP_LOOP:
            return stop_loop_(app);

        case FJ_APP_GET_LOOP_FLAGS:
            get_loop_flags_(data);

        case FJ_APP_PING:
            return ping_(app);

        default:
            return FJ_ERR_UNIMPLEMENTED;
    }
}
