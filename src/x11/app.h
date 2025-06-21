#include <src/shared/unix/events.h>

#include <fejix/app.h>

#include <X11/Xlib.h>
#include <xcb/xcb.h>


struct fj_app {
    void *userdata;

    struct fj_unix_events events;
    bool exit_requested;

    Display *display;
    xcb_connection_t *connection;
};
