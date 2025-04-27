#ifndef FEJIX_WINAPI_WINDOW_H_INCLUDED
#define FEJIX_WINAPI_WINDOW_H_INCLUDED


#include <fejix/modules/window.h>

#include <windows.h>


enum fj_winapi_window_flags {
    FJ_WINAPI_WINDOW_ABOUT_TO_CREATE = 1 << 0,
    FJ_WINAPI_WINDOW_WITH_VIEW = 1 << 1,
};


struct fj_window_manager {
    void *userdata;

    struct fj_io_thread *io_thread;
};

struct fj_window {
    void *userdata;

    HWND handle;
    enum fj_winapi_window_flags flags;

    struct fj_window_manager *manager;
};


#endif
