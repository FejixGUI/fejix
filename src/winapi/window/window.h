#ifndef FEJIX_WINAPI_WINDOW_H_
#define FEJIX_WINAPI_WINDOW_H_


#include <src/winapi/client/client.h>

#include <fejix/interface/window.h>


struct fj_window_manager {
    struct fj_client *client;
};


struct fj_window {
    union fj_tag tag;

    struct fj_window_manager *manager;

    HWND handle;
    ATOM class_atom;
};


#endif
