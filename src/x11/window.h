#ifndef FEJIX_X11_WINDOW_H_
#define FEJIX_X11_WINDOW_H_


#include <fejix/window.h>

#include <xcb/xcb.h>


struct fj_window_service {
    struct fj_window_service_base base;
};

struct fj_window {
    struct fj_window_base base;

    /** This is zero if the window has not been commited yet. */
    xcb_window_t id;

    xcb_visualid_t visual;

    /**
        If zero, the default colormap for the target screen is used.
        If non-zero, the colormap is freed when the window is deleted.
    */
    xcb_colormap_t colormap;

    uint8_t depth;
};


#endif
