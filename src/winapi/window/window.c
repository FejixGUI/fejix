#include <src/winapi/io_thread/io_thread.h>
#include <src/winapi/utils/window.h>
#include <src/winapi/window/window.h>

#include <fejix/utils/math.h>
#include <fejix/utils/memory.h>


static LONG_PTR __stdcall window_procedure(
    HWND handle, UINT message, UINT_PTR wparam, LONG_PTR lparam)
{
    enum fj_error e;

    if (message == WM_CREATE) {
        CREATESTRUCT *create_info = (void *) lparam;
        struct window *window = create_info->lpCreateParams;
        fj_winapi_window_set_data(handle, window);
    }

    struct fj_window *window = fj_winapi_window_get_data(handle);

    LONG_PTR result = 0;
    MSG msg = {
        .hwnd = handle,
        .message = message,
        .wParam = wparam,
        .lParam = lparam,
    };

    if (window->view != NULL) {
        e = fj_winapi_view_handle_event(window->view->manager, window->view, &msg, &result);

        if (e)  // TODO log errors here, I guess
            return DefWindowProc(handle, message, wparam, lparam);
    }

    return result;
}


enum fj_error fj_window_create_manager_winapi(
    struct fj_io_thread *io_thread, struct fj_window_manager **out_manager)
{
    enum fj_error e;

    e = FJ_ALLOC(out_manager);

    if (e)
        return e;

    (*out_manager)->io_thread = io_thread;

    return FJ_OK;
}


enum fj_error fj_window_destroy_manager_winapi(struct fj_window_manager *manager)
{
    (void) manager;
    return FJ_OK;
}


enum fj_error fj_window_create_winapi(
    struct fj_window_manager *manager, struct fj_window **out_window)
{
    enum fj_error e;

    e = FJ_ALLOC(out_window);

    if (e)
        return e;

    (*out_window)->manager = manager;

    WNDCLASSEX class_info = {
        .lpfnWndProc = window_procedure,
    };

    CREATESTRUCT window_info = {
        .style = WS_OVERLAPPEDWINDOW,
        .x = CW_USEDEFAULT,
        .y = CW_USEDEFAULT,
        .cx = CW_USEDEFAULT,
        .cy = CW_USEDEFAULT,
        .lpCreateParams = *out_window,
    };

    e = fj_winapi_window_create(&(*out_window)->handle, &class_info, &window_info);

    if (e) {
        FJ_FREE(out_window);
        return e;
    }

    return FJ_OK;
}


enum fj_error fj_window_destroy_winapi(struct fj_window_manager *manager, struct fj_window *window)
{
    (void) manager;
    enum fj_error e;

    e = fj_winapi_window_destroy(window->handle);

    if (e)
        return e;

    FJ_FREE(&window);

    return FJ_OK;
}


enum fj_error fj_window_sync_winapi(struct fj_window_manager *manager, struct fj_window *window)
{
    if (window->view != NULL) {
        fj_winapi_window_view_sync(window->view->manager, window->view);
    }

    return FJ_OK;
}
