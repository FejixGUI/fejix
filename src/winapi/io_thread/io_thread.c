#include <src/winapi/io_thread/io_thread.h>
#include <src/winapi/utils.h>

#include <fejix/utils/memory.h>

#include <math.h>


enum {
    INTERNAL_MESSAGE_IDLE = WM_USER,
    INTERNAL_MESSAGE_WAKEUP,
};


static void enable_dpi_awareness_for_process(void)
{
    // Using per-process awareness because some GPU drivers have bugs with per-thread awareness.
    // Using Set..DpiAwarenessContext because Set..DpiAwareness is buggy and inconsistent.
    // Bugs appear e.g. on laptops with integrated Intel and discrete NVidia GPUs.
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
}


/** \returns timeout in milliseconds. */
// static DWORD get_wakeup_timeout(struct fj_io_thread *thread)
// {
//     if (isinf(thread->wakeup_timeout) || thread->wakeup_timeout >= (double) INFINITE / 1000) {
//         return INFINITE;
//     }

//     return (DWORD) (thread->wakeup_timeout * 1000);
// }


static enum fj_error post_iteration_message(struct fj_io_thread *thread)
{
    BOOL result = SendNotifyMessageW(thread->message_only_window, INTERNAL_MESSAGE_IDLE, 0, 0);

    if (result == FALSE) {
        return FJ_ERROR_OPERATION_FAILED;
    }

    return FJ_OK;
}


static enum fj_error sleep(struct fj_io_thread *thread)
{
    DWORD result = MsgWaitForMultipleObjectsEx(
        0, NULL, app_get_wakeup_timeout(thread), QS_ALLINPUT, MWMO_INPUTAVAILABLE | MWMO_ALERTABLE);

    if (result == WAIT_FAILED) {
        return FJ_ERROR_IO_FAILED;
    }

    thread->wakeup_timeout = INFINITY;

    return FJ_OK;
}


static enum fj_error app_idle(struct fj_io_thread *thread)
{
    FJ_TRY (thread->callbacks->on_idle(thread)) {
        return fj_result;
    }

    if (thread->is_finished) {
        PostQuitMessage(0);
        return FJ_OK;
    }

    FJ_TRY (app_sleep(thread)) {
        return fj_result;
    }

    FJ_TRY (app_post_iteration_message(thread)) {
        return fj_result;
    }

    return FJ_OK;
}


static enum fj_error app_wakeup_immediately(struct fj_io_thread *thread)
{
    if (!SendNotifyMessage(thread->global_window, GLOBAL_MESSAGE_WAKEUP, 0, 0)) {
        return FJ_ERROR_REQUEST_SENDING_FAILED;
    }

    return FJ_OK;
}


static LRESULT CALLBACK
global_window_procedure(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam)
{
    struct fj_io_thread *thread = fj_winapi_window_get_data(window_handle);

    if (thread->is_finished) {
        return 0;
    }

    switch (message) {
        case GLOBAL_MESSAGE_ITERATE: {
            FJ_TRY (app_iterate(thread)) {
                PostQuitMessage((int) fj_result);
                return 0;
            }

            return 0;
        }

        case GLOBAL_MESSAGE_WAKEUP:
            return 0;

        case WM_QUERYENDSESSION:
            return !thread->is_critical;

        case WM_ENDSESSION: {
            BOOL session_is_being_ended = (BOOL) wparam;
            if (session_is_being_ended == FALSE) {
                // This is a feature of WinAPI: if WM_QUERYENDSESSION did some cleanup
                // (which it normally should not do), but the shutdown was cancelled,
                // the program state can be restored here in order to continue.
                return 0;
            }

            thread->callbacks->on_command(thread, fj_io_thread_COMMAND_FINISH);

            return 0;
        }

        default:
            return DefWindowProc(window_handle, message, wparam, lparam);
    }
}


static enum fj_error create_global_window(struct fj_io_thread *thread)
{
    WNDCLASSEX class_info = {
        .lpfnWndProc = global_window_procedure,
    };

    // Creating a normal window instead of a message-only window in order to receive broadcast
    // messages like WM_ENDSESSION.
    FJ_TRY (fj_winapi_window_create(&thread->global_window, &class_info, NULL)) {
        return fj_result;
    }

    fj_winapi_window_set_data(thread->global_window, thread);

    return FJ_OK;
}


static enum fj_error destroy_global_window(struct fj_io_thread *thread)
{
    return fj_winapi_window_destroy(thread->global_window);
}


static enum fj_error app_alloc(struct fj_io_thread **out_app)
{
    return FJ_ALLOC_ZEROED(out_app);
}


static enum fj_error app_destroy(struct fj_io_thread *thread)
{
    if (thread->global_window != NULL) {
        destroy_global_window(thread);
    }

    FJ_FREE(&thread);

    return FJ_OK;
}


static enum fj_error app_create(
    struct fj_io_thread **out_app, struct fj_io_thread_create_info const *info)
{
    FJ_TRY (FJ_ALLOC_ZEROED(out_app)) {
        return fj_result;
    }

    struct fj_io_thread *thread = *out_app;
    thread->tag = info->tag;
    thread->callbacks = info->callbacks;

    app_enable_high_dpi_for_process();

    FJ_TRY (create_global_window(thread)) {
        app_destroy(thread);
        return fj_result;
    }

    return FJ_OK;
}


static enum fj_error app_launch(struct fj_io_thread *thread)
{
    FJ_TRY (app_post_iteration_message(thread)) {
        return fj_result;
    }

    while (!thread->is_finished) {
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

    return thread->callbacks->on_command(thread, fj_io_thread_COMMAND_FINISH);
}


static enum fj_error app_manual_sleep(struct fj_io_thread *thread)
{
    FJ_TRY (app_sleep(thread)) {
        return fj_result;
    }

    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return FJ_OK;
}
