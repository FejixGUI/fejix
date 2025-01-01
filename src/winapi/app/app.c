#include <src/winapi/app/app.h>
#include <src/winapi/utils.h>

#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>

#include <math.h>


enum fj_winapi_global_message_id {
    FJ_WINAPI_GLOBAL_MESSAGE_ITERATE = WM_USER,
    FJ_WINAPI_GLOBAL_MESSAGE_WAKEUP,
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
    if (SendNotifyMessage(app->global_window, FJ_WINAPI_GLOBAL_MESSAGE_ITERATE, 0, 0) == FALSE) {
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
    if (!SendNotifyMessage(app->global_window, FJ_WINAPI_GLOBAL_MESSAGE_WAKEUP, 0, 0)) {
        return FJ_ERR_REQUEST_SENDING_FAILED;
    }

    return FJ_OK;
}


static LRESULT WINAPI global_window_procedure(
    HWND window_handle,
    UINT message,
    WPARAM wparam,
    LPARAM lparam
)
{
    struct fj_app *app = fj_winapi_get_window_data(window_handle);

    if (app->is_finished) {
        return 0;
    }

    switch (message) {
        case FJ_WINAPI_GLOBAL_MESSAGE_ITERATE: {
            FJ_TRY (app_iterate(app)) {
                PostQuitMessage((int) fj_result);
                return 0;
            }

            return 0;
        }

        case FJ_WINAPI_GLOBAL_MESSAGE_WAKEUP:
            return 0;

        default:
            return DefWindowProc(window_handle, message, wparam, lparam);
    }
}


static fj_err_t create_global_window(struct fj_app *app)
{
    WNDCLASSEX window_class = {
        .cbSize = sizeof(window_class),
        .hInstance = app->instance,
        .lpszClassName = TEXT("fejix_global_window_class"),
        .lpfnWndProc = global_window_procedure,
    };

    if (RegisterClassEx(&window_class) == 0) {
        return FJ_ERR_REQUEST_FAILED;
    }

    app->global_window = CreateWindow(
        window_class.lpszClassName, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, app->instance, NULL
    );

    if (app->global_window == NULL) {
        return FJ_ERR_REQUEST_FAILED;
    }

    fj_winapi_set_window_data(app->global_window, app);

    return FJ_OK;
}


static fj_err_t app_alloc(struct fj_app **out_app)
{
    return FJ_ALLOC_ZEROED(out_app);
}


static fj_err_t app_destroy(struct fj_app *app)
{
    if (app->global_window != NULL) {
        DestroyWindow(app->global_window);
    }

    FJ_FREE(&app);

    return FJ_OK;
}


static fj_err_t app_create(
    struct fj_app **out_app,
    struct fj_app_create_info const *info,
    struct fj_app_callbacks const *callbacks
)
{
    FJ_TRY (FJ_ALLOC_ZEROED(out_app)) {
        return fj_result;
    }

    struct fj_app *app = *out_app;
    *app = (struct fj_app) {
        .tag = info->tag,
        .callbacks = callbacks,
        .instance = GetModuleHandle(NULL),
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


static void const *app_get_extenion(fj_app_extension_id_t id)
{
    (void) id;
    return NULL;
}


struct fj_app_class const fj_winapi_app_class = {
    .get_extension = app_get_extenion,
    .create = app_create,
    .destroy = app_destroy,
    .launch = app_launch,
    .set_finished = app_set_finished,
    .wakeup_after_timeout = app_wakeup_after_timeout,
    .wakeup_immediately = app_wakeup_immediately,
};
