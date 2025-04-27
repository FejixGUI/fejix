#include <src/winapi/io_thread/io_thread.h>
#include <src/winapi/utils.h>
#include <src/winapi/window/window.h>

#include <fejix/utils/math.h>
#include <fejix/utils/memory.h>


static LRESULT __stdcall window_procedure(
    HWND handle, UINT message, UINT_PTR wparam, LONG_PTR lparam)
{
    if (message == WM_CREATE) {
        CREATESTRUCT *create_info = (void *) lparam;
        struct window *window = create_info->lpCreateParams;
        fj_winapi_window_set_data(handle, window);
    }

    struct fj_window *window = fj_winapi_window_get_data(handle);

    switch (message) {
        default:
            return DefWindowProc(handle, message, wparam, lparam);
    }
}


enum fj_error fj_window_create_manager_winapi(
    struct fj_io_thread *io_thread, struct fj_window_manager **out_manager)
{
    *out_manager = &io_thread->window_manager;
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

    e = FJ_ALLOC_ZEROED(out_window);

    if (e)
        return e;

    (*out_window)->manager = manager;
    (*out_window)->flags |= FJ_WINAPI_WINDOW_ABOUT_TO_CREATE;

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
    if (window->flags & FJ_WINAPI_WINDOW_ABOUT_TO_CREATE) {
        ShowWindow(window->handle, SW_SHOWNORMAL);
        window->flags &= ~FJ_WINAPI_WINDOW_ABOUT_TO_CREATE;
    }

    return FJ_OK;
}
