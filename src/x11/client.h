#ifndef FEJIX_INTERNAL_X11_CLIENT_H_
#define FEJIX_INTERNAL_X11_CLIENT_H_


#include "src/client.h"
#include "src/modules/fdpoll/fdpoll.h"

#include <xcb/xcb.h>
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>


struct fj_x11_data {
    fj_idstring_t client_id;

    xcb_connection_t * connection;
    Display * xdisplay;

    struct fj_fdpoll * fdpoll;
};


#endif
