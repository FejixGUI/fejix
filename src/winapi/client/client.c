#include <src/winapi/client/client.h>
#include <src/winapi/utils.h>

#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>


static fj_err_t client_handle_global_message(
    struct fj_client *client,
    struct fj_winapi_message const *message,
    LRESULT *result,
    fj_bool8_t *handled
)
{
    *handled = true;

    switch (message->message_type) {
        case FJ_WINAPI_USER_MESSAGE_SLEEP: {
            *result = 0;

            FJ_TRY (fj_winapi_scheduler_sleep(client)) {
                return fj_result;
            }
        }

        case FJ_WINAPI_USER_MESSAGE_WAKEUP: {
            *result = 0;
        }

        default:
            *handled = false;
    }

    return FJ_OK;
}


static fj_err_t client_handle_known_message(
    struct fj_client *client,
    struct fj_winapi_message const *message,
    LRESULT *result,
    fj_bool8_t *handled
)
{
    if (message->window == client->message_window) {
        return client_handle_global_message(client, message, result, handled);
    }

    return FJ_OK;
}


static fj_err_t client_handle_unknown_message(
    struct fj_client *client,
    struct fj_winapi_message const *message,
    LRESULT *result
)
{
    FJ_TRY (fj_winapi_scheduler_schedule_sleep(client)) {
        return fj_result;
    }

    *result = DefWindowProc(
        message->window, message->message_type, message->wparam, message->lparam
    );

    return FJ_OK;
}


static fj_err_t client_handle_message(
    struct fj_client *client,
    struct fj_winapi_message const *message,
    LRESULT *result
)
{
    fj_bool8_t handled = false;

    FJ_TRY (client_handle_known_message(client, message, result, &handled)) {
        return fj_result;
    }

    if (!handled) {
        return client_handle_unknown_message(client, message, result);
    }

    return FJ_OK;
}


static LRESULT client_handle_message_safely(
    struct fj_client *client,
    struct fj_winapi_message const *message
)
{
    if (client->message_processing_error != FJ_OK) {
        return 0;
    }

    LRESULT result = 0;
    FJ_TRY (client_handle_message(client, message, &result)) {
        client->message_processing_error = fj_result;
        PostQuitMessage(0);
    }

    return result;
}


LRESULT CALLBACK fj_winapi_window_procedure(
    HWND window,
    UINT message_type,
    WPARAM wparam,
    LPARAM lparam
)
{
    struct fj_winapi_window_data_base *window_data = fj_winapi_get_window_data(window);

    if (window_data == NULL) {
        return DefWindowProc(window, message_type, wparam, lparam);
    }

    struct fj_winapi_message message = {
        .window = window,
        .message_type = message_type,
        .wparam = wparam,
        .lparam = lparam,
    };

    return client_handle_message_safely(window_data->client, &message);
}


static fj_err_t create_message_window(struct fj_client *client)
{
    WNDCLASSEX window_class = {
        .cbSize = sizeof(window_class),
        .lpszClassName = TEXT("fejix_message_window"),
        .hInstance = client->instance,
        .lpfnWndProc = fj_winapi_window_procedure,
    };

    ATOM atom = RegisterClassEx(&window_class);

    if (atom == 0) {
        return FJ_ERR_REQUEST_FAILED;
    }

    client->message_window = CreateWindow(
        (void *) (uintptr_t) atom, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, client->instance, NULL
    );

    if (client->message_window == NULL) {
        return FJ_ERR_REQUEST_FAILED;
    }

    fj_winapi_set_window_data(client->message_window, &client->message_window_data);

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


static fj_err_t client_create(struct fj_client **client, struct fj_client_info const *info)
{
    FJ_TRY (FJ_ALLOC_ZEROED(client)) {
        return fj_result;
    }

    **client = (struct fj_client) {
        .tag = info->tag,
        .instance = GetModuleHandle(NULL),
        .message_window_data.client = *client,
    };

    FJ_TRY (create_message_window(*client)) {
        client_destroy(*client);
        return fj_result;
    }

    return FJ_OK;
}


static fj_err_t client_run(struct fj_client *client)
{
    FJ_TRY (fj_winapi_scheduler_schedule_sleep(client)) {
        return fj_result;
    }

    MSG msg;
    while (PeekMessage(&msg, NULL, 0, -1, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            break;
        }

        TranslateMessage(&msg);

        DispatchMessage(&msg);
        if (client->message_processing_error != FJ_OK) {
            return client->message_processing_error;
        }
    }

    return FJ_OK;
}


struct fj_client_iface const fj_winapi_client_iface = {
    .create = client_create,
    .destroy = client_destroy,
    .run = client_run,
};
