#include "app.h"

#include <src/shared/common/error.h>
#include <src/shared/common/memory.h>
#include <src/shared/common/task.h>

#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib-xcb.h>


#define EVENT_MASK (~0x80)  // Just a weirdness of XCB


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
        xcb_generic_event_t *event = xcb_poll_for_event(app->data->connection);

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
    fj_unix_events_deinit(&app->data->events);
}

static fj_err get_atoms(struct fj_app *app)
{
#undef FJ_MACROLIST
#undef FJ_MACROITEM
#define FJ_MACROITEM(ITEM) [FJ_X11_ATOM_##ITEM] = #ITEM,
#define FJ_MACROLIST(...) \
    static char const *const atom_names[FJ_X11_ATOM_MAX] = { __VA_ARGS__ };

    FJ_X11_ATOM_MACROLIST

    xcb_intern_atom_cookie_t coockies[FJ_X11_ATOM_MAX];

    for (enum fj_x11_atom atom = 0; atom < FJ_X11_ATOM_MAX; atom++) {
        coockies[atom] = xcb_intern_atom(
            app->data->connection,
            1,
            strlen(atom_names[atom]),
            atom_names[atom]);
    }

    xcb_flush(app->data->connection);

    for (enum fj_x11_atom atom = 0; atom < FJ_X11_ATOM_MAX; atom++) {
        xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(
            app->data->connection, coockies[atom], NULL);

        app->data->atoms[atom] = reply->atom;

        free(reply);
    }

    return FJ_OK;
}


static fj_err app_deinit(struct fj_app *app)
{
    deinit_events(app);

    if (app->data->display != NULL) {
        int result = XCloseDisplay(app->data->display);
        if (result != 0) {
            FJ_ERRORF(
                "cannot close X11 display: %s",
                fj_x11_error_into_string(result));
        }
    }

    FJ_FREE(&app->data);

    return FJ_OK;
}


static int xlib_error_handler(Display *display, XErrorEvent *error)
{
    (void) display;
    FJ_ERRORF("Xlib error: %s", fj_x11_error_into_string(error->error_code));
    return 0;
}

static fj_err init_connection(struct fj_app *app)
{
    fj_err e;

    app->data->display = XOpenDisplay(NULL);
    if (app->data->display == NULL) {
        FJ_ERROR("cannot open X11 display");
        return FJ_ERR_SYSTEM;
    }

    app->data->connection = XGetXCBConnection(app->data->display);
    XSetEventQueueOwner(app->data->display, XCBOwnsEventQueue);
    XSetErrorHandler(xlib_error_handler);
    get_atoms(app);

    e = fj_unix_events_add(
        &app->data->events,
        xcb_get_file_descriptor(app->data->connection),
        POLLIN,
        process_events);
    if (e)
        return e;

    return FJ_OK;
}


static fj_err handle_ping(void *callback_data, int fd, short events)
{
    struct fj_app *app = callback_data;

    app->dispatch(app, FJ_APP_DID_PING, NULL, NULL);

    return fj_unix_events_handle_ping(fd, events);
}


static void main_task_poll(struct fj_task *task)
{
    struct fj_app *app = (void *) task->data;

    fj_err e = fj_unix_events_process(&app->data->events);
    if (e) {
        // TODO do we need to shut everything down in case of errors?
        fj_task_set_completed(task, e);
        app->user_task.cancel(&app->user_task);
        return;
    }

    if (!app->user_task.completed)
        app->user_task.poll(&app->user_task);
}

static void main_task_cancel(struct fj_task *task)
{
    struct fj_app *app = (void *) task->data;

    fj_task_set_completed(task, FJ_ERR_CANCELED);

    if (!app->user_task.completed)
        app->user_task.cancel(&app->user_task);
}

static fj_err app_init(struct fj_app *app)
{
    fj_err e;

    e = FJ_ALLOC(&app->data);
    if (e)
        return e;

    e = fj_unix_events_init(&app->data->events, app, handle_ping);
    if (e)
        return e;

    fj_task_set_pending(
        &app->main_task, (uintptr_t) app, main_task_poll, main_task_cancel);

    e = init_connection(app);
    if (e) {
        app_deinit(app);
        return e;
    }

    app->dispatch(app, FJ_APP_DID_SET_SYSTEM_HANDLE, app->data->display, NULL);

    enum fj_app_task_flags flags = FJ_APP_TASK_FLAG_AWAITABLE
        | FJ_APP_TASK_FLAG_POLLABLE | FJ_APP_TASK_FLAG_CANCELABLE;
    app->dispatch(app, FJ_APP_DID_SET_TASK_FLAGS, &flags, NULL);

    return FJ_OK;
}


static fj_err await(struct fj_app *app, struct fj_task *task)
{
    // Here we are trying to complete the specified task without processing
    // the events as soon as possible. If the task completes on the second
    // poll (like the window update tasks or a dummy task that is programmed
    // that way), the event loop (the main task) is not touched.

    fj_err e;

    if (task) {
        task->poll(task);

        if (task->completed)
            return FJ_OK;
    }

    for (;;) {
        e = fj_unix_events_wait(&app->data->events, NULL);
        if (e)
            return e;

        if (task) {
            task->poll(task);

            if (task->completed)
                return FJ_OK;
        }

        app->main_task.poll(&app->main_task);

        if (app->main_task.completed)
            return FJ_OK;
    }

    return FJ_OK;
}


static fj_err ping(struct fj_app *app)
{
    return fj_unix_events_ping(&app->data->events);
}


fj_err fj_x11_app_dispatcher(void *app, int32_t message, void *data)
{
    switch (message) {
        case FJ_APP_INIT:
            return app_init(app);

        case FJ_APP_DEINIT:
            return app_deinit(app);

        case FJ_APP_AWAIT:
            return await(app, data);

        case FJ_APP_PING:
            return ping(app);

        default:
            return FJ_ERR_UNIMPLEMENTED;
    }
}
