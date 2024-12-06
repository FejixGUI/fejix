#include <src/winapi/client/client.h>
#include <src/winapi/utils.h>

#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>


enum fj_winapi_message_window_message_id {
    FJ_WINAPI_MESSAGE_WINDOW_MESSAGE_SLEEP = WM_USER,
    FJ_WINAPI_MESSAGE_WINDOW_MESSAGE_WAKEUP,
};


static DWORD client_get_sleep_timeout(struct fj_client *client)
{
    if (client->is_idle_requested) {
        return 0;
    }

    if (client->wait_timeout == 0) {
        return INFINITE;
    }

    DWORD timeout_ms = FJ_TIMEOUT_INTO_MILLIS(client->wait_timeout);

    return FJ_MAX(1, timeout_ms);
}


static fj_err_t client_schedule_sleep(struct fj_client *client)
{
    if (SendNotifyMessage(client->message_window, FJ_WINAPI_MESSAGE_WINDOW_MESSAGE_SLEEP, 0, 0)
        == FALSE)
    {
        return FJ_ERR_REQUEST_FAILED;
    }

    return FJ_OK;
}


static fj_err_t client_sleep(struct fj_client *client)
{
    FJ_TRY (client->callbacks.idle(client)) {
        return fj_result;
    }

    if (client->is_quit_requested) {
        PostQuitMessage(0);
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
    if (!SendNotifyMessage(client->message_window, FJ_WINAPI_MESSAGE_WINDOW_MESSAGE_WAKEUP, 0, 0)) {
        return FJ_ERR_REQUEST_SENDING_FAILED;
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

    if (client->is_quit_requested) {
        return 0;
    }

    switch (message) {
        case FJ_WINAPI_MESSAGE_WINDOW_MESSAGE_SLEEP: {
            FJ_TRY (client_sleep(client)) {
                PostQuitMessage(0);
                return 0;
            }

            return 0;
        }

        case FJ_WINAPI_MESSAGE_WINDOW_MESSAGE_WAKEUP:
            return 0;

        default:
            return DefWindowProc(window_handle, message, wparam, lparam);
    }
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

    while (!client->is_quit_requested) {
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


static void client_request_quit(struct fj_client *client)
{
    client->is_quit_requested = true;
}

static void client_request_idle(struct fj_client *client)
{
    client->is_idle_requested = true;
}


static void client_wait_timeout_set_timeout(struct fj_client *client, fj_timeout_t timeout)
{
    client->wait_timeout = FJ_CLAMP(
        timeout, FJ_TIMEOUT_FROM_MILLIS(1), FJ_TIMEOUT_FROM_MILLIS(INFINITE - 1)
    );
}

static void client_wait_timeout_unset_timeout(struct fj_client *client)
{
    client->wait_timeout = 0;
}


struct fj_client_interface const fj_winapi_client_interface = {
    .create = client_create,
    .destroy = client_destroy,
    .run = client_run,
    .request_quit = client_request_quit,
    .request_idle = client_request_idle,
    .wakeup = client_wakeup,
};

struct fj_client_wait_timeout_interface const fj_winapi_client_wait_timeout_interface = {
    .set_timeout = client_wait_timeout_set_timeout,
    .unset_timeout = client_wait_timeout_unset_timeout,
};
