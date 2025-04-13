#include <src/unix/events.h>

#include <fejix/modules/app.h>
#include <fejix/modules/clock.h>

#include <X11/Xlib-xcb.h>
#include <X11/Xlib.h>
#include <xcb/xcb.h>


struct fj_app_manager {
    void *userdata;
    fj_app_event_callback callback;

    struct fj_unix_events events;
    bool finish_requested;

    Display *display;
    xcb_connection_t *connection;
};
