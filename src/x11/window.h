#ifndef FEJIX_X11_WINDOW_H_
#define FEJIX_X11_WINDOW_H_


#include <xcb/xcb.h>

#include <fejix/window.h>

#include <src/shared/common/list.h>


FJ_LIST(fj_window_list, struct fj_window *)

struct fj_window_service_private_data
{
    struct fj_window_list windows;
};


struct fj_window_private_data
{
    xcb_window_t id;
};


#endif
