#include <src/x11/window.h>

#include <src/shared/unix/events.h>

#include <fejix/app.h>

#include <X11/Xlib.h>
#include <xcb/xcb.h>


enum fj_x11_atom {
    FJ_X11_ATOM_NET_WM_NAME,
    FJ_X11_ATOM_UTF8_STRING,
    FJ_X11_ATOM_MAX,
};


struct fj_app {
    struct fj_app_base base;

    struct fj_unix_events events;
    bool should_quit;

    Display *display;
    xcb_connection_t *connection;
    xcb_atom_t atoms[FJ_X11_ATOM_MAX];

    struct fj_window_service window_service;
};


int fj_x11_xlib_get_last_error(void);
void fj_x11_xlib_clear_last_error(void);
