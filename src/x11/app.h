#ifndef FEJIX_X11_APP_H_
#define FEJIX_X11_APP_H_

#include <fejix/app.h>

#include <src/x11/x11.h>

#include <src/shared/common/macros.h>
#include <src/shared/unix/events.h>


struct fj_app_data
{
    struct fj_unix_events events;
    Display *display;
    xcb_connection_t *connection;
    xcb_atom_t atoms[FJ_X11_ATOM_COUNT];

    // struct fj_window_service window_service;
};


#endif
