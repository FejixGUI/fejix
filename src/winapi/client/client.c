#include <src/winapi/client/client.h>

#include <fejix/interface/client.h>

#include <fejix/core/utils.h>
#include <fejix/core/malloc.h>

#include <math.h>


enum fj_winapi_wait_result {
    FJ_WINAPI_WAIT_RESULT_TIMEOUT,
    FJ_WINAPI_WAIT_RESULT_MESSAGE,
};


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
fj_err_t client_wait_message_or_timeout(struct fj_winapi_client * this, DWORD /*out*/ * wait_result)
{
    *wait_result = MsgWaitForMultipleObjectsEx(
        0,
        NULL,
        convert_timeout(this->timeout),
        QS_ALLINPUT,
        MWMO_INPUTAVAILABLE
    );

    if (*wait_result == WAIT_FAILED) {
        return FJ_ERR_IO_ERROR;
    }

    return FJ_OK;
}


static
fj_bool32_t client_next_message(struct fj_winapi_client * this, MSG /*out*/ * msg)
{
    if (isnan(this->timeout)) {
        return false;
    }

    BOOL has_new_message = PeekMessage(msg, NULL, 0, 0, PM_REMOVE);

    if (has_new_message && msg->message == WM_QUIT) {
        return false;
    }

    return has_new_message;
}


static
fj_err_t client_dispatch_messages(struct fj_winapi_client * this)
{
    MSG msg;
    while (client_next_message(this, &msg)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (this->event_handling_error != FJ_OK) {
            return this->event_handling_error;
        }
    }

    return FJ_OK;
}


static
fj_err_t client_create(
    fj_client_t */*? out*/ * _this,
    struct fj_client_callbacks const * callbacks,
    void * callback_data,
    struct fj_client_info const * info
)
{
    FJ_ARG_FROM_OPAQUE(_this, struct fj_winapi_client */*? out*/ * this)
    FJ_ARG_UNUSED(info)

    FJ_INIT_TRY

    FJ_TRY(FJ_ALLOC_ZEROED(this)) {
        return FJ_RESULT;
    }

    (*this)->callbacks = callbacks;
    (*this)->data = callback_data;
    (*this)->instance = GetModuleHandle(NULL);
    (*this)->main_thread_id = GetCurrentThreadId();

    ensure_created_thread_event_queue();

    return FJ_OK;
}


static
fj_err_t client_destroy(fj_client_t * this)
{
    FJ_FREE(&this);

    return FJ_OK;
}





static
fj_err_t client_run(fj_client_t * _this, fj_client_run_type_t run_type, void * run_data)
{
    FJ_ARG_FROM_OPAQUE(_this, struct fj_winapi_client * this)
    FJ_ARG_UNUSED(run_data)

    FJ_INIT_TRY

    if (run_type != FJ_CLIENT_RUN_TYPE_MAIN) {
        return FJ_OK;
    }

    while (!isnan(this->timeout)) {
        DWORD wait_result;
        FJ_TRY(client_wait_message_or_timeout(this, &wait_result)) {
            return FJ_RESULT;
        }

        switch (wait_result) {
            case FJ_WINAPI_WAIT_RESULT_MESSAGE:
                FJ_TRY(client_dispatch_messages(this)) {
                    return FJ_RESULT;
                }
            break;

            case FJ_WINAPI_WAIT_RESULT_TIMEOUT:
                FJ_TRY(this->callbacks->idle(this->data)) {
                    return FJ_RESULT;
                }
            break;
        }

    }

    return FJ_OK;
}


static
void client_set_timeout(fj_client_t * _this, fj_seconds_t timeout)
{
    FJ_ARG_FROM_OPAQUE(_this, struct fj_winapi_client * this)

    this->timeout = timeout;
}


static
fj_err_t client_wakeup(fj_client_t * _this)
{
    FJ_ARG_FROM_OPAQUE(_this, struct fj_winapi_client * this)

    BOOL result = PostThreadMessage(this->main_thread_id, WM_USER, 0, NULL);

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
