#include <src/winapi/app/app.h>
#include <src/winapi/utils.h>

#include <fejix/app.h>
#include <fejix/app_manual_sleep.h>

#include <fejix/core/utils.h>

#include <math.h>


enum global_message_id {
    GLOBAL_MESSAGE_ITERATE = WM_USER,
    GLOBAL_MESSAGE_WAKEUP,
};


static void app_enable_high_dpi_for_process(void)
{
    // Using per-process awareness because some GPU drivers have bugs with per-thread awareness.
    // Using Set..DpiAwarenessContext because Set..DpiAwareness is buggy and inconsistent.
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
}


/** Returns timeout in milliseconds. */
static DWORD app_get_wakeup_timeout(struct fj_app *app)
{
    if (isinf(app->wakeup_timeout) || app->wakeup_timeout >= (double) INFINITE / 1000) {
        return INFINITE;
    }

    return (DWORD) (app->wakeup_timeout * 1000);
}


static fj_err_t app_post_iteration_message(struct fj_app *app)
{
    if (SendNotifyMessage(app->global_window, GLOBAL_MESSAGE_ITERATE, 0, 0) == FALSE) {
        return FJ_ERR_REQUEST_FAILED;
    }

    return FJ_OK;
}


static fj_err_t app_sleep(struct fj_app *app)
{
    DWORD wait_result = MsgWaitForMultipleObjectsEx(
        0, NULL, app_get_wakeup_timeout(app), QS_ALLINPUT, MWMO_INPUTAVAILABLE | MWMO_ALERTABLE
    );

    if (wait_result == WAIT_FAILED) {
        return FJ_ERR_EVENT_WAITING_FAILED;
    }

    app->wakeup_timeout = INFINITY;

    return FJ_OK;
}


static fj_err_t app_iterate(struct fj_app *app)
{
    FJ_TRY (app->callbacks->on_idle(app)) {
        return fj_result;
    }

    if (app->is_finished) {
        PostQuitMessage(0);
        return FJ_OK;
    }

    FJ_TRY (app_sleep(app)) {
        return fj_result;
    }


    FJ_TRY (app_post_iteration_message(app)) {
        return fj_result;
    }

    return FJ_OK;
}


static fj_err_t app_wakeup_immediately(struct fj_app *app)
{
    if (!SendNotifyMessage(app->global_window, GLOBAL_MESSAGE_WAKEUP, 0, 0)) {
        return FJ_ERR_REQUEST_SENDING_FAILED;
    }

    return FJ_OK;
}


static LRESULT CALLBACK global_window_procedure(
    HWND window_handle,
    UINT message,
    WPARAM wparam,
    LPARAM lparam
)
{
    struct fj_app *app = fj_winapi_window_get_data(window_handle);

    if (app->is_finished) {
        return 0;
    }

    switch (message) {
        case GLOBAL_MESSAGE_ITERATE: {
            FJ_TRY (app_iterate(app)) {
                PostQuitMessage((int) fj_result);
                return 0;
            }

            return 0;
        }

        case GLOBAL_MESSAGE_WAKEUP:
            return 0;

        default:
            return DefWindowProc(window_handle, message, wparam, lparam);
    }
}


static fj_err_t create_global_window(struct fj_app *app)
{
    struct fj_winapi_window_info window_info = {
        .procedure = global_window_procedure,
        .parent = HWND_MESSAGE,
    };

    FJ_TRY (fj_winapi_window_create_simple(&app->global_window, &window_info)) {
        return fj_result;
    }

    fj_winapi_window_set_data(app->global_window, app);

    return FJ_OK;
}


static fj_err_t destroy_global_window(struct fj_app *app)
{
    return fj_winapi_window_destroy_simple(app->global_window);
}


static fj_err_t app_alloc(struct fj_app **out_app)
{
    return FJ_ALLOC_ZEROED(out_app);
}


static fj_err_t app_destroy(struct fj_app *app)
{
    if (app->global_window != NULL) {
        destroy_global_window(app);
    }

    FJ_FREE(&app);

    return FJ_OK;
}


static fj_err_t app_create(struct fj_app **out_app, struct fj_app_create_info const *info)
{
    FJ_TRY (FJ_ALLOC_ZEROED(out_app)) {
        return fj_result;
    }

    struct fj_app *app = *out_app;
    *app = (struct fj_app) {
        .tag = info->tag,
        .callbacks = info->callbacks,
    };

    app_enable_high_dpi_for_process();

    FJ_TRY (create_global_window(app)) {
        app_destroy(app);
        return fj_result;
    }

    return FJ_OK;
}


static fj_err_t app_launch(struct fj_app *app)
{
    FJ_TRY (app_post_iteration_message(app)) {
        return fj_result;
    }

    while (!app->is_finished) {
        MSG msg;
        if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            continue;
        }

        if (msg.message == WM_QUIT) {
            break;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return app->callbacks->on_force(app, FJ_APP_FORCE_FINISH);
}


static fj_err_t app_manual_sleep(struct fj_app *app)
{
    FJ_TRY (app_sleep(app)) {
        return fj_result;
    }

    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return FJ_OK;
}


static void app_set_finished(struct fj_app *app)
{
    app->is_finished = true;
}


static void app_wakeup_after_timeout(struct fj_app *app, fj_seconds_t timeout)
{
    app->wakeup_timeout = timeout;
}


static struct fj_app_manual_sleep_funcs const app_manual_sleep_funcs = {
    .manual_sleep = app_manual_sleep,
};


static void const *app_get_interface_funcs(fj_app_interface_id_t id)
{
    switch (id) {
        case FJ_APP_INTERFACE_MANUAL_SLEEP:
            return &app_manual_sleep_funcs;
    }

    return NULL;
}

static fj_app_implementation_id_t app_get_implementation_id(void)
{
    return FJ_APP_IMPLEMENTATION_WINAPI;
}

static void app_get_implementation_version(struct fj_version *out_version)
{
    *out_version = (struct fj_version) {
        .major = 0,
        .minor = 0,
        .patch = 1,
    };
}


struct fj_app_funcs const fj_winapi_app_funcs = {
    .get_implementation_id = app_get_implementation_id,
    .get_implementation_version = app_get_implementation_version,
    .get_interface_funcs = app_get_interface_funcs,
    .create = app_create,
    .destroy = app_destroy,
    .launch = app_launch,
    .set_finished = app_set_finished,
    .wakeup_after_timeout = app_wakeup_after_timeout,
    .wakeup_immediately = app_wakeup_immediately,
};
