#include "window.h"

#include <src/x11/app.h>

#include <src/shared/utils/logging.h>
#include <src/shared/utils/memory.h>

#include <malloc.h>


static enum fj_error fj_window_service_new_(
    struct fj_window_service **out_service, struct fj_app *app)
{
    fj_window_service_init_base(&app->window_service, app);
    *out_service = &app->window_service;
    return FJ_OK;
}

static enum fj_error fj_window_service_del_(struct fj_window_service *service)
{
    (void) service;
    return FJ_OK;
}


static enum fj_error fj_window_del_(struct fj_window *window)
{
    xcb_connection_t *c = window->base.app->connection;

    if (window->colormap) {
        xcb_void_cookie_t coockie = xcb_free_colormap_checked(c, window->colormap);
        xcb_generic_error_t *error = xcb_request_check(c, coockie);
        if (error) {
            FJ_WARN("xcb_free_colormap failed");
            free(error);
            // Proceed to window deletion
        }
    }

    if (window->id) {
        xcb_void_cookie_t coockie = xcb_destroy_window_checked(c, window->id);
        xcb_generic_error_t *error = xcb_request_check(c, coockie);

        if (error) {
            FJ_ERROR("xcb_destroy_window failed");
            free(error);
            return FJ_ERROR_OPERATION_FAILED;
        }
    }

    FJ_FREE(&window);
    return FJ_OK;
}


static enum fj_error fj_window_new_(
    struct fj_window **out_window, struct fj_window_service *service)
{
    enum fj_error e;

    struct fj_window *window;
    e = FJ_ALLOC(&window);
    if (e)
        return e;

    fj_window_init_base(window, service);

    *out_window = window;
    return FJ_OK;
}


static enum fj_error create_window(struct fj_app *app, struct fj_window *window)
{
    // TODO Add a module to enumarate screens and allow screen specification here
    xcb_screen_t *default_screen = xcb_setup_roots_iterator(xcb_get_setup(app->connection)).data;

    xcb_colormap_t colormap = window->colormap;

    if (colormap == 0) {
        colormap = default_screen->default_colormap;
    }

    if (window->depth == 0) {
        window->depth = default_screen->root_depth;
    }

    if (window->visual == 0) {
        window->visual = default_screen->root_visual;
    }

    // TODO Research whether XCB_CW_BACKING_STORE is useful
    uint32_t property_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;

    uint32_t properties[] = {
        default_screen->black_pixel,

        XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_KEY_PRESS
            | XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_FOCUS_CHANGE | XCB_EVENT_MASK_BUTTON_PRESS
            | XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION
            | XCB_EVENT_MASK_BUTTON_MOTION | XCB_EVENT_MASK_ENTER_WINDOW
            | XCB_EVENT_MASK_LEAVE_WINDOW | XCB_EVENT_MASK_VISIBILITY_CHANGE,

        colormap,
    };

    // TODO Add preferred size retrieval here
    xcb_void_cookie_t coockie = xcb_create_window_checked(
        app->connection,
        XCB_COPY_FROM_PARENT,
        window->id,
        default_screen->root,
        0,
        0,
        100,
        70,
        0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        window->visual,
        property_mask,
        properties);
    xcb_generic_error_t *error = xcb_request_check(app->connection, coockie);
    if (error) {
        FJ_ERROR("xcb_create_window failed");
        free(error);
        return FJ_ERROR_OPERATION_FAILED;
    }

    // TODO Should windows be initially visible or hidden? If visible, then add cached visiblity
    // state retrieval here.

    // TODO Add initial events (size, orientation, density, visibility) here

    return FJ_OK;
}


static enum fj_error fj_window_commit_(struct fj_window *window)
{
    if (window->id == 0) {
        return create_window(window->base.service->base.app, window);
    }

    return FJ_OK;
}

// TODO

void fj_x11_init_window(void)
{
    FJ_API_INIT(fj_window_service_new)
    FJ_API_INIT(fj_window_service_del)
    FJ_API_INIT(fj_window_new)
    FJ_API_INIT(fj_window_del)
    FJ_API_INIT(fj_window_commit)
}
