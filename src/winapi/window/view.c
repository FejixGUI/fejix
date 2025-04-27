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


enum fj_error fj_window_view_create_manager_winapi(
    struct fj_window_manager *window_manager, struct fj_window_view_manager **out_manager)
{
    enum fj_error e;

    e = FJ_ALLOC_ZEROED(out_manager);

    if (e)
        return e;

    (*out_manager)->userdata = NULL;
    (*out_manager)->window_manager = window_manager;

    return FJ_OK;
}


enum fj_error fj_window_view_destroy_manager_winapi(struct fj_window_view_manager *manager)
{
    (void) manager;
    return FJ_OK;
}

enum fj_error fj_window_view_set_callback_winapi(
    struct fj_window_view_manager *manager, fj_window_view_event_callback callback)
{
    (void) manager;
    (void) callback;
    return FJ_OK;
}


enum fj_error fj_window_view_create_winapi(
    struct fj_window_view_manager *manager, struct fj_window_view **out_view)
{
    enum fj_error e;

    e = FJ_ALLOC_ZEROED(out_view);

    if (e)
        return e;


    return FJ_OK;
}

FJ_METHOD(
    fj_window_view_destroy,
    enum fj_error,
    struct fj_window_view_manager *manager,
    struct fj_window_view *view)

FJ_METHOD(
    fj_window_view_set_hint,
    enum fj_error,
    struct fj_window_view_manager *manager,
    struct fj_window_view *view,
    enum fj_window_view_hint_type hint_type,
    union fj_window_view_hint_data hint_data)
