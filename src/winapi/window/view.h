#ifndef FEJIX_WINAPI_WINDOW_VIEW_H_INCLUDED
#define FEJIX_WINAPI_WINDOW_VIEW_H_INCLUDED


#include <src/winapi/window/window.h>

#include <fejix/modules/window_view.h>


struct fj_window_view { };

struct fj_window_view_manager {
    void *userdata;

    struct fj_window_manager *window_manager;
};


#endif
