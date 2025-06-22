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
    void *userdata;

    struct fj_unix_events events;
    bool exit_requested;

    Display *display;
    xcb_connection_t *connection;
    xcb_atom_t atoms[FJ_X11_ATOM_MAX];
};


int fj_x11_xlib_get_last_error(void);
void fj_x11_xlib_clear_last_error(void);
