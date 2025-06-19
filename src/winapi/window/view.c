#include <src/winapi/window/view.h>

#include <fejix/utils/memory.h>


enum {
    /**
        lparam contains *HDWP - a pointer to a DeferWindowPos structure handle.
        If a window needs to change its layout through DeferWindowPos, it should do
        BeginDeferWindowPos (if the handle is NULL) and then DeferWindowPos saving the result into
        the handle behind the pointer.
        the handle behind the pointer.
    */
    INTERNAL_MESSAGE_WINDOW_CONTENT_UPDATE = WM_USER,
};

enum fj_status fj_winapi_window_view_handle_event(
    struct fj_window_view_manager *manager,
    struct fj_window_view *view,
    MSG const *msg,
    LONG_PTR *result)
{
    *result = 0;

    switch (msg->message) {
            // TODO window messages

        case WM_DESTROY: {
            view->manager->callback(
                view->manager,
                FJ_WINDOW_VIEW_EVENT_DISMISS,
                (union fj_window_view_event_data) { 0 });

            return FJ_STATUS_OK;
        }

        default: {
            *result = DefWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
            return FJ_STATUS_OK;
        }
    }
}


enum fj_status fj_window_view_create_manager_winapi(
    struct fj_drawable_manager *window_manager, struct fj_window_view_manager **out_manager)
{
    enum fj_status e;

    e = FJ_ALLOC(out_manager);

    if (e)
        return e;

    (*out_manager)->userdata = NULL;
    (*out_manager)->window_manager = window_manager;

    return FJ_STATUS_OK;
}


enum fj_status fj_window_view_destroy_manager_winapi(struct fj_window_view_manager *manager)
{
    (void) manager;
    return FJ_STATUS_OK;
}


void fj_window_view_set_callback_winapi(
    struct fj_window_view_manager *manager, fj_window_view_event_callback callback)
{
    manager->callback = callback;
}


enum fj_status fj_window_view_create_winapi(
    struct fj_window_view_manager *manager,
    struct fj_drawable *window,
    struct fj_window_view **out_view)
{
    enum fj_status e;

    e = FJ_ALLOC(out_view);

    if (e)
        return e;

    (*out_view)->window = window;

    return FJ_STATUS_OK;
}


enum fj_status fj_window_view_destroy_winapi(
    struct fj_window_view_manager *manager, struct fj_window_view *view)
{
    (void) manager;

    FJ_FREE(&view);

    return FJ_STATUS_OK;
}


enum fj_status fj_window_view_set_hint_winapi(
    struct fj_window_view_manager *manager,
    struct fj_window_view *view,
    enum fj_window_view_hint_type hint_type,
    union fj_window_view_hint_data hint_data)
{
    return FJ_STATUS_OK;
}

enum fj_status fj_window_view_request_content_winapi(
    struct fj_window_view_manager *manager, struct fj_window_view *view)
{
    return FJ_STATUS_OK;
}


enum fj_status fj_winapi_window_view_sync(
    struct fj_window_view_manager *manager, struct fj_window_view *view)
{
    if (view->flags & FJ_WINAPI_WINDOW_VIEW_SHOW) {
        ShowWindow(view->window->handle, SW_SHOWNORMAL);
    }

    return FJ_STATUS_OK;
}
