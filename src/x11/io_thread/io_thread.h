#include <src/unix/events.h>

#include <fejix/modules/io_thread.h>
#include <fejix/modules/timer.h>

#include <X11/Xlib-xcb.h>
#include <X11/Xlib.h>
#include <xcb/xcb.h>


struct fj_io_thread {
    void *userdata;
    fj_io_thread_event_callback callback;

    struct fj_unix_events events;
    bool finish_requested;

    Display *display;
    xcb_connection_t *connection;
};
