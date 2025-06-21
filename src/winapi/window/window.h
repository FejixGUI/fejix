#ifndef FEJIX_WINAPI_WINDOW_H_
#define FEJIX_WINAPI_WINDOW_H_

#include <src/winapi/window/view.h>

#include <fejix/modules/window.h>

#include <windows.h>


struct fj_window_manager {
    void *userdata;

    struct fj_io_thread *io_thread;
    struct fj_window_view_manager *view_manager;
};

struct fj_window {
    void *userdata;

    struct fj_window_manager *manager;

    struct fj_window_view *view;

    HWND handle;
};


#endif
