#include <src/winapi/client/client.h>

#include <fejix/interface/client.h>

#include <fejix/core/utils.h>
#include <fejix/core/alloc.h>

#include <math.h>


static
void ensure_created_thread_event_queue(void)
{
    MSG msg;
    PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
}


static
DWORD convert_timeout(fj_seconds_t timeout)
{
    if (isinf(timeout)) {
        return INFINITE;
    }

    return (DWORD) timeout;
}

static
fj_err_t client_wait_message_or_timeout(
    struct fj_winapi_client * client,
    DWORD /*out*/ * wait_result
)
{
    *wait_result = MsgWaitForMultipleObjectsEx(
        0,
        NULL,
        convert_timeout(client->timeout),
        QS_ALLINPUT,
        MWMO_INPUTAVAILABLE
    );

    if (*wait_result == WAIT_FAILED) {
        return FJ_ERR_IO_ERROR;
    }

    return FJ_OK;
}


static
fj_bool32_t client_next_message(struct fj_winapi_client * client, MSG /*out*/ * msg)
{
    if (isnan(client->timeout)) {
        return false;
    }

    BOOL has_new_message = PeekMessage(msg, NULL, 0, 0, PM_REMOVE);

    if (has_new_message && msg->message == WM_QUIT) {
        return false;
    }

    return has_new_message;
}


static
fj_err_t client_dispatch_messages(struct fj_winapi_client * client)
{
    MSG msg;
    while (client_next_message(client, &msg)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (client->event_handling_error != FJ_OK) {
            return client->event_handling_error;
        }
    }

    return FJ_OK;
}


static
fj_err_t client_create(
    struct fj_client */*? out*/ * client_,
    struct fj_client_callbacks const * callbacks,
    void * callback_data,
    struct fj_client_info const * _info
)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_winapi_client */*? out*/ *)
    FJ_ARG_UNUSED(info)

        FJ_TRY(FJ_ALLOC_ZEROED(client)) {
        return fj_result;
    }

    (*client)->callbacks = callbacks;
    (*client)->data = callback_data;
    (*client)->instance = GetModuleHandle(NULL);
    (*client)->main_thread_id = GetCurrentThreadId();

    ensure_created_thread_event_queue();

    return FJ_OK;
}


static
fj_err_t client_destroy(struct fj_client * client)
{
    FJ_FREE(&client);

    return FJ_OK;
}





static
fj_err_t client_run(struct fj_client * client_, fj_client_run_type_t run_type, void * _run_data)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_winapi_client *)
    FJ_ARG_UNUSED(run_data)

        if (run_type != FJ_CLIENT_RUN_MAIN) {
        return FJ_OK;
    }

    FJ_TRY(client->callbacks->idle(client->data)) {
        return fj_result;
    }

    while (!isnan(client->timeout)) {
        DWORD wait_result;
        FJ_TRY(client_wait_message_or_timeout(client, &wait_result)) {
            return fj_result;
        }

        if (wait_result == WAIT_OBJECT_0) {
            FJ_TRY(client_dispatch_messages(client)) {
                return fj_result;
            }
        }

        FJ_TRY(client->callbacks->idle(client->data)) {
            return fj_result;
        }
    }

    return FJ_OK;
}


static
void client_set_timeout(struct fj_client * client_, fj_seconds_t timeout)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_winapi_client *)

    client->timeout = timeout;
}


static
fj_err_t client_wakeup(struct fj_client * client_)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_winapi_client *)

    BOOL result = PostThreadMessage(client->main_thread_id, WM_USER, 0, NULL);

    if (result == FALSE) {
        return FJ_ERR_MESSAGE_SEND_ERROR;
    }

    return FJ_OK;
}


struct fj_client_iface const fj_winapi_client_impl = {
    .create = client_create,
    .destroy = client_destroy,
    .run = client_run,
    .set_timeout = client_set_timeout,
    .wakeup = client_wakeup,
};
