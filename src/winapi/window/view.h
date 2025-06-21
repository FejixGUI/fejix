#ifndef FEJIX_WINAPI_WINDOW_VIEW_H_
#define FEJIX_WINAPI_WINDOW_VIEW_H_


#include <src/winapi/window/window.h>

#include <fejix/modules/window_view.h>

#include <windows.h>


enum fj_window_view_flags {
    FJ_WINAPI_WINDOW_VIEW_SHOW = 1 << 0,
};


struct fj_window_view {
    void *userdata;

    struct fj_window_view_manager *manager;

    struct fj_window *window;
    enum fj_window_view_flags flags;
};

struct fj_window_view_manager {
    void *userdata;

    fj_window_view_event_callback callback;

    struct fj_window_manager *window_manager;
};


enum fj_error fj_winapi_view_handle_event(
    struct fj_window_view_manager *manager,
    struct fj_window_view *view,
    MSG const *msg,
    LONG_PTR *result);

enum fj_error fj_winapi_window_view_sync(
    struct fj_window_view_manager *manager, struct fj_window_view *view);


#endif
