#include "window.h"

#include <src/x11/app.h>
#include <src/x11/x11.h>

#include <src/shared/common/error.h>
#include <src/shared/common/memory.h>
#include <src/shared/common/task.h>


enum
{
    NORMAL_WINDOW_EVENT_MASK = XCB_EVENT_MASK_EXPOSURE
        | XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_KEY_PRESS
        | XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_FOCUS_CHANGE
        | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE
        | XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_BUTTON_MOTION
        | XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW
        | XCB_EVENT_MASK_VISIBILITY_CHANGE,
};


static fj_err window_add_to_service(struct fj_window *window)
{
    return fj_window_list_append(&window->service->data->windows, &window);
}

static fj_err window_remove_from_service(struct fj_window *window)
{
    struct fj_window_list *windows = &window->service->data->windows;

    for (uint32_t i = 0; i < windows->length; i++) {
        if (windows->items[i] == window) {
            return fj_window_list_remove(windows, i);
        }
    }

    return FJ_OK;
}


fj_err window_deinit(struct fj_window *window)
{
    fj_err e;

    struct fj_app_private_data *app_data = window->service->app->data;
    xcb_connection_t *c = app_data->connection;

    if (window->data->id) {
        e = fj_x11_check(c, xcb_destroy_window_checked(c, window->data->id));
        if (e)
            return e;
    }

    window_remove_from_service(window);
    FJ_FREE(&window);

    return FJ_OK;
}


fj_err window_init(struct fj_window *window)
{
    fj_err e;

    struct fj_app_private_data *app_data = window->service->app->data;
    xcb_connection_t *c = app_data->connection;

    e = FJ_ALLOC(&window->data);
    if (e)
        return e;

    e = window_add_to_service(window);
    if (e) {
        window_deinit(window);
        return e;
    }

    window->data->id = xcb_generate_id(c);

    // TODO Add preferred size retrieval here

    struct fj_x11_create_window_data data = {
        .window = window->data->id,
        .viewport = { { 0, 0 }, { 500, 400 } },
        .event_mask = NORMAL_WINDOW_EVENT_MASK,
    };

    e = fj_x11_check(c, fj_x11_create_window(c, NULL, &data));
    if (e) {
        window_deinit(window);
        return e;
    }

    xcb_atom_t protocols_property = app_data->atoms[FJ_X11_ATOM(WM_PROTOCOLS)];
    xcb_atom_t protocols[] = {
        app_data->atoms[FJ_X11_ATOM(_NET_WM_SYNC_REQUEST)],
        app_data->atoms[FJ_X11_ATOM(WM_DELETE_WINDOW)],
    };

    e = fj_x11_check(
        c,
        xcb_change_property_checked(
            c,
            XCB_PROP_MODE_REPLACE,
            window->data->id,
            protocols_property,
            XCB_ATOM_ATOM,
            sizeof(*protocols) * 8 /*bits*/,
            FJ_LEN(protocols),
            protocols));
    if (e) {
        window_deinit(window);
        return e;
    }

    e = fj_x11_check(c, xcb_map_window_checked(c, window->data->id));
    if (e) {
        window_deinit(window);
        return e;
    }

    // TODO Add initial events (size, orientation, density, visibility) here

    return FJ_OK;
}


fj_err fj_x11_window_dispatch(
    void *window, fj_message message, void *data, struct fj_task *task)
{
    fj_err e;

    switch (message) {
        case FJ_WINDOW_INIT:
            e = window_init(window);
            fj_task_set_completed(task, e);
            return e;

        case FJ_WINDOW_DEINIT:
            e = window_deinit(window);
            fj_task_set_completed(task, e);
            return e;

        default:
            return FJ_ERR_UNIMPLEMENTED;
    }
}
