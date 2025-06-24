#ifndef FEJIX_X11_WINDOW_H_
#define FEJIX_X11_WINDOW_H_


#include <fejix/window.h>

#include <xcb/xcb.h>


struct fj_window {
    xcb_window_t window;
    xcb_visualid_t visualid;
    xcb_colormap_t colormap;
};


#endif
