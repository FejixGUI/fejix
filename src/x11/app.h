#include <src/shared/unix/events.h>

#include <fejix/app.h>
#include <fejix/timer.h>

#include <X11/Xlib-xcb.h>
#include <X11/Xlib.h>
#include <xcb/xcb.h>


struct fj_io_thread {
    void *userdata;

    struct fj_unix_events events;
    bool quit_requested;

    Display *display;
    xcb_connection_t *connection;
};
