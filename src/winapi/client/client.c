#include <src/winapi/client/client.h>
#include <src/winapi/utils.h>

#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>

#include <math.h>


enum fj_winapi_global_message_id {
    FJ_WINAPI_GLOBAL_MESSAGE_ITERATE = WM_USER,
    FJ_WINAPI_GLOBAL_MESSAGE_WAKEUP,
};


static void client_enable_high_dpi_for_process(void)
{
    // Using per-process awareness because some GPU drivers have bugs with per-thread awareness.
    // Using Set..DpiAwarenessContext because Set..DpiAwareness is buggy and inconsistent.
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
}


/** Returns timeout in milliseconds. */
static DWORD client_get_wakeup_timeout(struct fj_client *client)
{
    if (isinf(client->wakeup_timeout) || client->wakeup_timeout >= (double) INFINITE / 1000) {
        return INFINITE;
    }

    return (DWORD) (client->wakeup_timeout * 1000);
}


static fj_err_t client_post_iteration_message(struct fj_client *client)
{
    if (SendNotifyMessage(client->global_window, FJ_WINAPI_GLOBAL_MESSAGE_ITERATE, 0, 0) == FALSE) {
        return FJ_ERR_REQUEST_FAILED;
    }

    return FJ_OK;
}


static fj_err_t client_sleep(struct fj_client *client)
{
    DWORD wait_result = MsgWaitForMultipleObjectsEx(
        0,
        NULL,
        client_get_wakeup_timeout(client),
        QS_ALLINPUT,
        MWMO_INPUTAVAILABLE | MWMO_ALERTABLE
    );

    if (wait_result == WAIT_FAILED) {
        return FJ_ERR_EVENT_WAITING_FAILED;
    }

    client->wakeup_timeout = 0.0;

    return FJ_OK;
}


static fj_err_t client_iterate(struct fj_client *client)
{
    FJ_TRY (FJ_RESPOND(client, FJ_CLIENT_IDLE, NULL, NULL)) {
        return fj_result;
    }

    if (client->is_finished) {
        PostQuitMessage(0);
        return FJ_OK;
    }

    FJ_TRY (client_sleep(client)) {
        return fj_result;
    }


    FJ_TRY (client_post_iteration_message(client)) {
        return fj_result;
    }

    return FJ_OK;
}


static fj_err_t client_wakeup_instantly(struct fj_client *client)
{
    if (!SendNotifyMessage(client->global_window, FJ_WINAPI_GLOBAL_MESSAGE_WAKEUP, 0, 0)) {
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
    struct fj_client *client = fj_winapi_get_window_data(window_handle);

    if (client->is_finished) {
        return 0;
    }

    switch (message) {
        case FJ_WINAPI_GLOBAL_MESSAGE_ITERATE: {
            FJ_TRY (client_iterate(client)) {
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


static fj_err_t create_global_window(struct fj_client *client)
{
    WNDCLASSEX window_class = {
        .cbSize = sizeof(window_class),
        .hInstance = client->instance,
        .lpszClassName = TEXT("fejix_schedule_window_class"),
        .lpfnWndProc = global_window_procedure,
    };

    if (RegisterClassEx(&window_class) == 0) {
        return FJ_ERR_REQUEST_FAILED;
    }

    client->global_window = CreateWindow(
        window_class.lpszClassName, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, client->instance, NULL
    );

    if (client->global_window == NULL) {
        return FJ_ERR_REQUEST_FAILED;
    }

    fj_winapi_set_window_data(client->global_window, client);

    return FJ_OK;
}


static fj_err_t client_alloc(struct fj_client **out_client)
{
    return FJ_ALLOC_ZEROED(out_client);
}


static void client_dealloc(struct fj_client *client)
{
    FJ_FREE(&client);
}


static void client_deinit(struct fj_client *client)
{
    if (client->global_window != NULL) {
        DestroyWindow(client->global_window);
    }
}


static fj_err_t client_init(struct fj_client *client)
{
    client_enable_high_dpi_for_process();

    client->instance = GetModuleHandle(NULL);

    FJ_TRY (create_global_window(client)) {
        client_deinit(client);
        return fj_result;
    }

    return FJ_OK;
}


static fj_err_t client_launch(struct fj_client *client)
{
    FJ_TRY (client_post_iteration_message(client)) {
        return fj_result;
    }

    while (!client->is_finished) {
        MSG msg;
        if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            continue;
        }

        if (msg.message == WM_QUIT) {
            return FJ_OK;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return FJ_OK;
}


static fj_err_t client_block_for_events(struct fj_client *client)
{
    FJ_TRY (client_sleep(client)) {
        return fj_result;
    }

    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return FJ_OK;
}


static void client_set_finished(struct fj_client *client)
{
    client->is_finished = true;
}


static void client_schedule_wakeup(
    struct fj_client *client,
    struct fj_client_schedule_wakeup_info const *info
)
{
    client->wakeup_timeout = info ? info->timeout : 0;
}


static void client_get_implementation_id(
    struct fj_client *client,
    fj_client_implementation_id_t *out_id
)
{
    *out_id = FJ_CLIENT_IMPLEMENTATION_WINAPI;
}


static void client_get_implementation_version(
    struct fj_client *client,
    struct fj_version *out_version
)
{
    *out_version = (struct fj_version) {
        .major = 0,
        .minor = 0,
        .patch = 1,
    };
}


fj_err_t fj_client_responder(void *obj, fj_request_id_t req, void const *in, void *out)
{
    switch (req) {
        case FJ_CLIENT_GET_IMPLEMENTATION_ID:
            client_get_implementation_id(obj, out);
            return FJ_OK;

        case FJ_CLIENT_GET_IMPLEMENTATION_VERSION:
            client_get_implementation_version(obj, out);
            return FJ_OK;

        case FJ_CLIENT_ALLOC:
            return client_alloc(out);

        case FJ_CLIENT_DEALLOC:
            client_dealloc(obj);
            return FJ_OK;

        case FJ_CLIENT_INIT:
            return client_init(obj);

        case FJ_CLIENT_DEINIT:
            client_deinit(obj);
            return FJ_OK;

        case FJ_CLIENT_LAUNCH:
            return client_launch(obj);

        case FJ_CLIENT_IDLE:
            return FJ_OK;

        case FJ_CLIENT_SCHEDULE_WAKEUP:
            client_schedule_wakeup(obj, in);
            return FJ_OK;

        case FJ_CLIENT_WAKEUP_INSTANTLY:
            return client_wakeup_instantly(obj);

        case FJ_CLIENT_SET_FINISHED:
            client_set_finished(obj);
            return FJ_OK;

        case FJ_CLIENT_BLOCK_FOR_EVENTS:
            return client_block_for_events(obj);

        default:
            return FJ_ERR_UNSUPPORTED;
    }
}
