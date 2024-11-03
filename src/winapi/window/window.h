#ifndef FEJIX_WINAPI_WINDOW_H_
#define FEJIX_WINAPI_WINDOW_H_


#include <fejix/interface/window.h>

#include <windows.h>


struct fj_window_manager {
    struct fj_client *client;
};


struct fj_window {
    union fj_tag tag;

    struct fj_window_manager *manager;

    struct fj_canvas *canvas;

    /** Can be NULL when the window is uninitialized. */
    HWND handle;

    /** Can be 0 when the window is uninitialized. */
    ATOM class_atom;
};


#endif
