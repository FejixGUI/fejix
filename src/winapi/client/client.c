#include <src/winapi/client/client.h>
#include <src/winapi/utils.h>

#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>


static fj_bool8_t client_needs_quit(struct fj_client *client)
{
    return client->is_quit_requested || client->message_processing_result != FJ_OK;
}


static DWORD client_get_sleep_timeout(struct fj_client *client)
{
    if (client->is_idle_requested) {
        return 0;
    }

    fj_timeout_t timeout = fj_winapi_sleep_timer_get_timeout(client);

    if (timeout == 0) {
        return INFINITE;
    }

    DWORD timeout_ms = FJ_TIMEOUT_INTO_MILLIS(timeout);

    return FJ_MAX(1, timeout_ms);
}


static fj_err_t client_schedule_sleep(struct fj_client *client)
{
    if (SendNotifyMessage(client->message_window, FJ_WINAPI_USER_MESSAGE_SLEEP, 0, 0) == FALSE) {
        return FJ_ERR_REQUEST_FAILED;
    }

    return FJ_OK;
}


static void client_schedule_quit(struct fj_client *client)
{
    client->is_quit_requested = true;
    PostQuitMessage(0);
}


static fj_err_t client_sleep(struct fj_client *client)
{
    FJ_TRY (client->callbacks.idle(client)) {
        return fj_result;
    }

    if (client->is_quit_requested) {
        client_schedule_quit(client);
        return FJ_OK;
    }

    DWORD wait_result = MsgWaitForMultipleObjectsEx(
        0, NULL, client_get_sleep_timeout(client), QS_ALLINPUT, MWMO_INPUTAVAILABLE | MWMO_ALERTABLE
    );

    if (wait_result == WAIT_FAILED) {
        return FJ_ERR_EVENT_WAITING_FAILED;
    }

    FJ_TRY (client_schedule_sleep(client)) {
        return fj_result;
    }

    return FJ_OK;
}


static fj_err_t client_wakeup(struct fj_client *client)
{
    if (!SendNotifyMessage(client->message_window, FJ_WINAPI_USER_MESSAGE_WAKEUP, 0, 0)) {
        return FJ_ERR_REQUEST_SENDING_FAILED;
    }

    return FJ_OK;
}


LONG_PTR fj_winapi_client_handle_message_safely(
    struct fj_client *client,
    MSG const *message,
    fj_err_t (*handle_message)(struct fj_client *client, MSG const *message, LONG_PTR *result)
)
{
    if (client_needs_quit(client)) {
        return 0;
    }

    LONG_PTR result = 0;
    FJ_TRY (handle_message(client, message, &result)) {
        client->message_processing_result = fj_result;
        client_schedule_quit(client);
        return result;
    }

    return result;
}


static fj_err_t message_window_handle_message(
    struct fj_client *client,
    MSG const *message,
    LONG_PTR *result
)
{
    switch (message->message) {
        case FJ_WINAPI_USER_MESSAGE_SLEEP: {
            *result = 0;

            FJ_TRY (client_sleep(client)) {
                return fj_result;
            }
            break;
        }

        case FJ_WINAPI_USER_MESSAGE_WAKEUP: {
            *result = 0;
            break;
        }

        default: {
            fj_winapi_handle_unknown_message(message, result);
            break;
        }
    }

    return FJ_OK;
}


static LONG_PTR __stdcall message_window_procedure(
    HWND window_handle,
    UINT message,
    UINT_PTR wparam,
    LONG_PTR lparam
)
{
    struct fj_client *client = fj_winapi_get_window_data(window_handle);
    MSG msg = { window_handle, message, wparam, lparam };
    return fj_winapi_client_handle_message_safely(client, &msg, message_window_handle_message);
}


static fj_err_t create_message_window(struct fj_client *client)
{
    WNDCLASSEX window_class = {
        .cbSize = sizeof(window_class),
        .hInstance = client->instance,
        .lpszClassName = TEXT("fejix_message_window_class"),
        .lpfnWndProc = message_window_procedure,
    };

    if (RegisterClassEx(&window_class) == 0) {
        return FJ_ERR_REQUEST_FAILED;
    }

    client->message_window = CreateWindow(
        window_class.lpszClassName, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, client->instance, NULL
    );

    if (client->message_window == NULL) {
        return FJ_ERR_REQUEST_FAILED;
    }

    fj_winapi_set_window_data(client->message_window, client);

    return FJ_OK;
}


static fj_err_t client_destroy(struct fj_client *client)
{
    if (client->message_window != NULL) {
        DestroyWindow(client->message_window);
    }

    FJ_FREE(&client);

    return FJ_OK;
}


static fj_err_t client_create(
    struct fj_client **client,
    struct fj_client_callbacks const *callbacks,
    struct fj_client_create_info const *create_info
)
{
    FJ_TRY (FJ_ALLOC_ZEROED(client)) {
        return fj_result;
    }

    **client = (struct fj_client) {
        .tag = create_info->tag,
        .callbacks = *callbacks,
        .instance = GetModuleHandle(NULL),
    };

    FJ_TRY (create_message_window(*client)) {
        client_destroy(*client);
        return fj_result;
    }

    return FJ_OK;
}


static fj_err_t client_run(struct fj_client *client)
{
    FJ_TRY (client_schedule_sleep(client)) {
        return fj_result;
    }

    while (!client_needs_quit(client)) {
        MSG msg;
        if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            continue;
        }

        if (msg.message == WM_QUIT) {
            return client->message_processing_result;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return client->message_processing_result;
}


static void client_request_quit(struct fj_client *client)
{
    client->is_quit_requested = true;
}

static void client_request_idle(struct fj_client *client)
{
    client->is_idle_requested = true;
}


struct fj_client_funcs const fj_winapi_client_funcs = {
    .create = client_create,
    .destroy = client_destroy,
    .run = client_run,
    .request_quit = client_request_quit,
    .request_idle = client_request_idle,
    .wakeup = client_wakeup,
};
