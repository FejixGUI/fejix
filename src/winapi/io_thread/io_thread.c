#include <src/winapi/io_thread/io_thread.h>
#include <src/winapi/utils/window.h>

#include <fejix/utils/math.h>
#include <fejix/utils/memory.h>

#include <math.h>


enum {
    INTERNAL_MESSAGE_ITERATE = WM_USER,
    INTERNAL_MESSAGE_PING_SELF,
};


static void enable_dpi_awareness_for_process(void)
{
    // Using per-process awareness because some GPU drivers have bugs with per-thread awareness.
    // Using Set..DpiAwarenessContext because Set..DpiAwareness is buggy and inconsistent.
    // Bugs appear e.g. on laptops with integrated Intel + NVidia GPUs.
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
}


static enum fj_error post_sleep_message(struct fj_io_thread *io_thread)
{
    BOOL result = SendNotifyMessageW(io_thread->message_window, INTERNAL_MESSAGE_ITERATE, 0, 0);

    if (result == FALSE) {
        return FJ_ERROR_OPERATION_FAILED;
    }

    return FJ_OK;
}


static enum fj_error sleep(struct fj_io_thread *io_thread, DWORD duration)
{
    (void) io_thread;

    DWORD result = MsgWaitForMultipleObjectsEx(
        0, NULL, duration, QS_ALLINPUT, MWMO_INPUTAVAILABLE | MWMO_ALERTABLE);

    if (result == WAIT_FAILED) {
        return FJ_ERROR_IO_FAILED;
    }

    return FJ_OK;
}


static enum fj_error iterate(struct fj_io_thread *io_thread)
{
    enum fj_error e;

    union fj_io_thread_event_data data = { 0 };
    io_thread->callback(io_thread, FJ_IO_THREAD_EVENT_NO_MORE_EVENTS, data);

    e = sleep(io_thread, fj_winapi_timer_manager_get_sleep_duration(&io_thread->timer_manager));

    if (e)
        return e;

    e = post_sleep_message(io_thread);

    if (e)
        return e;

    return FJ_OK;
}


static LONG_PTR __stdcall handle_message_window_event(
    HWND window_handle, UINT message, UINT_PTR wparam, LONG_PTR lparam)
{
    enum fj_error e;

    struct fj_io_thread *io_thread = fj_winapi_window_get_data(window_handle);

    if (io_thread->finished) {
        return 0;
    }

    switch (message) {
        case INTERNAL_MESSAGE_ITERATE: {
            e = iterate(io_thread);

            if (e) {
                PostQuitMessage((int) e);
                return 0;
            }

            return 0;
        }

        case INTERNAL_MESSAGE_PING_SELF:
            return 0;

        case WM_ENDSESSION: {
            BOOL session_is_being_ended = (BOOL) wparam;
            if (session_is_being_ended == FALSE) {
                // This is a feature of WinAPI: if theoratically WM_QUERYENDSESSION does cleanup
                // (which it normally should not do), but the shutdown was cancelled,
                // the program state can be restored here in order to continue.
                return 0;
            }

            union fj_io_thread_event_data data = { 0 };
            io_thread->callback(io_thread, FJ_IO_THREAD_EVENT_FINISH, data);
            io_thread->callback(io_thread, FJ_IO_THREAD_EVENT_NO_MORE_EVENTS, data);

            return 0;
        }

        default:
            return DefWindowProc(window_handle, message, wparam, lparam);
    }
}


static enum fj_error create_message_window(struct fj_io_thread *io_thread)
{
    enum fj_error e;

    WNDCLASSEX class_info = {
        .lpfnWndProc = handle_message_window_event,
    };

    // Creating a normal window instead of a message-only window in order to receive broadcast
    // messages like WM_ENDSESSION.
    e = fj_winapi_window_create(&io_thread->message_window, &class_info, NULL);

    if (e)
        return e;

    fj_winapi_window_set_data(io_thread->message_window, io_thread);

    return FJ_OK;
}


static enum fj_error destroy_message_window(struct fj_io_thread *io_thread)
{
    return fj_winapi_window_destroy(io_thread->message_window);
}


enum fj_error fj_io_thread_destroy_winapi(struct fj_io_thread *io_thread)
{
    if (io_thread->message_window != NULL) {
        destroy_message_window(io_thread);
    }

    FJ_FREE(&io_thread);

    return FJ_OK;
}


enum fj_error fj_io_thread_create_winapi(struct fj_io_thread **out_io_thread)
{
    enum fj_error e;

    e = FJ_ALLOC(out_io_thread);

    if (e)
        return e;

    (*out_io_thread)->callback = fj_io_thread_event_callback_default;

    enable_dpi_awareness_for_process();

    e = create_message_window(*out_io_thread);

    if (e) {
        fj_io_thread_destroy_winapi(*out_io_thread);
        return e;
    }

    return FJ_OK;
}


void fj_io_thread_set_callback_winapi(
    struct fj_io_thread *io_thread, fj_io_thread_event_callback callback)
{
    io_thread->callback = callback;
}


enum fj_error fj_io_thread_start_winapi(struct fj_io_thread *io_thread)
{
    enum fj_error e;

    e = post_sleep_message(io_thread);

    if (e)
        return e;

    while (!io_thread->finished) {
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

    union fj_io_thread_event_data data = { 0 };
    io_thread->callback(io_thread, FJ_IO_THREAD_EVENT_FINISH, data);

    return FJ_OK;
}


enum fj_error fj_io_thread_ping_self_winapi(struct fj_io_thread *io_thread)
{
    if (!SendNotifyMessage(io_thread->message_window, INTERNAL_MESSAGE_PING_SELF, 0, 0)) {
        return FJ_ERROR_IO_FAILED;
    }

    return FJ_OK;
}


enum fj_error fj_io_thread_finish_winapi(struct fj_io_thread *io_thread)
{
    io_thread->finished = true;
    PostQuitMessage(0);
    return FJ_OK;
}


enum fj_error fj_io_thread_sleep_create_manager_winapi(
    struct fj_io_thread *io_thread, struct fj_io_thread_sleep_manager **out_manager)
{
    *out_manager = &io_thread->sleep_manager;
    (*out_manager)->io_thread = io_thread;

    return FJ_OK;
}


enum fj_error fj_io_thread_sleep_destroy_manager_winapi(struct fj_io_thread_sleep_manager *manager)
{
    (void) manager;
    return FJ_OK;
}


enum fj_error fj_io_thread_sleep_winapi(struct fj_io_thread *thread, fj_time const *opt_duration)
{
    enum fj_error e;

    DWORD duration = opt_duration != NULL ? fj_time_into_millis(*opt_duration) : INFINITE;

    if (duration != 0) {
        e = sleep(thread, duration);

        if (e)
            return e;
    }

    MSG msg;

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return FJ_OK;
}
