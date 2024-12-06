#ifndef FEJIX_WINAPI_WINDOW_H_
#define FEJIX_WINAPI_WINDOW_H_


#include <fejix/interface/window.h>

#include <windows.h>


struct fj_window_manager {
    struct fj_client *client;
};

struct fj_window_builder {
    WNDCLASSEX window_class;
    DWORD style;
    DWORD exstyle;

    /** NULL until the window gets created. */
    struct fj_window *window;
};

struct fj_window {
    union fj_tag tag;

    struct fj_window_manager *manager;

    HWND handle;

    ATOM class_atom;
};


#endif
