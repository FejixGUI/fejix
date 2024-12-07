#ifndef FEJIX_WINAPI_WINDOW_H_
#define FEJIX_WINAPI_WINDOW_H_


#include <fejix/interface/window.h>

#include <windows.h>


struct fj_window_manager {
    struct fj_client *client;
};

struct fj_window_builder {
    WNDCLASSEX window_class;
    ATOM window_class_atom;
    DWORD window_style;
    DWORD window_exstyle;
    union fj_tag window_tag;
};

struct fj_window {
    union fj_tag tag;

    struct fj_window_manager *manager;

    HWND handle;
    ATOM class_atom;
};


#endif
