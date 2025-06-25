#include "app.h"

#include <src/shared/utils/logging.h>
#include <src/shared/utils/memory.h>

#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib-xcb.h>


#define EVENT_MASK (~0x80)  // Just a weirdness of XCB


static uint8_t xlib_last_error = Success;

static int xlib_error_handler(Display *display, XErrorEvent *error)
{
    (void) display;
    xlib_last_error = error->error_code;
    return 0;
}

uint8_t fj_x11_xlib_get_last_error(void)
{
    return xlib_last_error;
}

void fj_x11_xlib_clear_last_error(void)
{
    xlib_last_error = Success;
}

char const *fj_x11_error_into_string(uint8_t error_code)
{
    static char const *const messages[] = {
        [Success] = "Success: everything's okay",
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

char const *fj_x11_xcb_error_into_string(xcb_generic_error_t *error)
{
    return fj_x11_error_into_string(error->error_code);
}


static enum fj_error process_event(struct fj_app *app, xcb_generic_event_t *event)
{
    // TODO delegate window events to some function in window.c
    switch (event->response_type & EVENT_MASK) {
        default:
            break;

        case XCB_CLIENT_MESSAGE: {
            xcb_client_message_event_t *client_message = (void *) event;
            uint32_t *data = client_message->data.data32;
            uint32_t message_type = data[0];

            for (uint32_t i = 0; i < app->window_service.windows.length; i++) {
                if (app->window_service.windows.items[i]->id == client_message->window) {
                    if (message_type == FJ_X11_GET_ATOM(app, WM_DELETE_WINDOW)) {
                        fj_window_close_callback(app->window_service.windows.items[i]);
                    }
                    break;
                }
            }
        }
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
#define FJ_X11_ATOM_LIST_ITEM(X) [FJ_X11_ATOM_ID(X)] = #X,
    static char const *const atom_names[FJ_X11_ATOM_MAX] = { FJ_X11_ATOM_LIST };
#undef FJ_X11_ATOM_LIST_ITEM

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
        if (result != 0) {
            FJ_ERROR("cannot close X11 display: %s", fj_x11_error_into_string(result));
        }
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

    fj_app_start_callback(app);

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


static enum fj_error fj_app_quit_(struct fj_app *app)
{
    app->should_quit = true;
    return FJ_OK;
}


static enum fj_error fj_app_ping_(struct fj_app *app)
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
