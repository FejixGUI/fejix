#include <src/winapi/connection/connection.h>
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
// static DWORD get_wakeup_timeout(struct fj_connection *conn)
// {
//     if (isinf(conn->wakeup_timeout) || conn->wakeup_timeout >= (double) INFINITE / 1000) {
//         return INFINITE;
//     }

//     return (DWORD) (conn->wakeup_timeout * 1000);
// }


static enum fj_error post_iteration_message(struct fj_connection *conn)
{
    BOOL result = SendNotifyMessageW(conn->message_only_window, INTERNAL_MESSAGE_IDLE, 0, 0);

    if (result == FALSE) {
        return FJ_ERROR_OPERATION_FAILED;
    }

    return FJ_OK;
}


static enum fj_error sleep(struct fj_connection *conn)
{
    DWORD result = MsgWaitForMultipleObjectsEx(
        0, NULL, app_get_wakeup_timeout(conn), QS_ALLINPUT, MWMO_INPUTAVAILABLE | MWMO_ALERTABLE);

    if (result == WAIT_FAILED) {
        return FJ_ERROR_IO_FAILED;
    }

    conn->wakeup_timeout = INFINITY;

    return FJ_OK;
}


static enum fj_error app_idle(struct fj_connection *conn)
{
    FJ_TRY (conn->callbacks->on_idle(conn)) {
        return fj_result;
    }

    if (conn->is_finished) {
        PostQuitMessage(0);
        return FJ_OK;
    }

    FJ_TRY (app_sleep(conn)) {
        return fj_result;
    }

    FJ_TRY (app_post_iteration_message(conn)) {
        return fj_result;
    }

    return FJ_OK;
}


static enum fj_error app_wakeup_immediately(struct fj_connection *conn)
{
    if (!SendNotifyMessage(conn->global_window, GLOBAL_MESSAGE_WAKEUP, 0, 0)) {
        return FJ_ERROR_REQUEST_SENDING_FAILED;
    }

    return FJ_OK;
}


static LRESULT CALLBACK
global_window_procedure(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam)
{
    struct fj_connection *conn = fj_winapi_window_get_data(window_handle);

    if (conn->is_finished) {
        return 0;
    }

    switch (message) {
        case GLOBAL_MESSAGE_ITERATE: {
            FJ_TRY (app_iterate(conn)) {
                PostQuitMessage((int) fj_result);
                return 0;
            }

            return 0;
        }

        case GLOBAL_MESSAGE_WAKEUP:
            return 0;

        case WM_QUERYENDSESSION:
            return !conn->is_critical;

        case WM_ENDSESSION: {
            BOOL session_is_being_ended = (BOOL) wparam;
            if (session_is_being_ended == FALSE) {
                // This is a feature of WinAPI: if WM_QUERYENDSESSION did some cleanup
                // (which it normally should not do), but the shutdown was cancelled,
                // the program state can be restored here in order to continue.
                return 0;
            }

            conn->callbacks->on_command(conn, fj_connection_COMMAND_FINISH);

            return 0;
        }

        default:
            return DefWindowProc(window_handle, message, wparam, lparam);
    }
}


static enum fj_error create_global_window(struct fj_connection *conn)
{
    WNDCLASSEX class_info = {
        .lpfnWndProc = global_window_procedure,
    };

    // Creating a normal window instead of a message-only window in order to receive broadcast
    // messages like WM_ENDSESSION.
    FJ_TRY (fj_winapi_window_create(&conn->global_window, &class_info, NULL)) {
        return fj_result;
    }

    fj_winapi_window_set_data(conn->global_window, conn);

    return FJ_OK;
}


static enum fj_error destroy_global_window(struct fj_connection *conn)
{
    return fj_winapi_window_destroy(conn->global_window);
}


static enum fj_error app_alloc(struct fj_connection **out_app)
{
    return FJ_ALLOC_ZEROED(out_app);
}


static enum fj_error app_destroy(struct fj_connection *conn)
{
    if (conn->global_window != NULL) {
        destroy_global_window(conn);
    }

    FJ_FREE(&conn);

    return FJ_OK;
}


static enum fj_error app_create(
    struct fj_connection **out_app, struct fj_connection_create_info const *info)
{
    FJ_TRY (FJ_ALLOC_ZEROED(out_app)) {
        return fj_result;
    }

    struct fj_connection *conn = *out_app;
    conn->tag = info->tag;
    conn->callbacks = info->callbacks;

    app_enable_high_dpi_for_process();

    FJ_TRY (create_global_window(conn)) {
        app_destroy(conn);
        return fj_result;
    }

    return FJ_OK;
}


static enum fj_error app_launch(struct fj_connection *conn)
{
    FJ_TRY (app_post_iteration_message(conn)) {
        return fj_result;
    }

    while (!conn->is_finished) {
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

    return conn->callbacks->on_command(conn, fj_connection_COMMAND_FINISH);
}


static enum fj_error app_manual_sleep(struct fj_connection *conn)
{
    FJ_TRY (app_sleep(conn)) {
        return fj_result;
    }

    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return FJ_OK;
}
