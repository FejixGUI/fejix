#ifndef FEJIX_X11_APP_H_
#define FEJIX_X11_APP_H_

#include <src/shared/unix/events.h>

#include <fejix/app.h>

#include <X11/Xlib.h>
#include <xcb/xcb.h>


/**
    Define FJ_X11_ATOM_LIST_ITEM to make this do whatever you want.

    The atoms are sorted alphabetically here.
*/
#define FJ_X11_ATOM_LIST                                \
    FJ_X11_ATOM_LIST_ITEM(_NET_WM_NAME)                 \
    FJ_X11_ATOM_LIST_ITEM(_NET_WM_SYNC_REQUEST)         \
    FJ_X11_ATOM_LIST_ITEM(_NET_WM_SYNC_REQUEST_COUNTER) \
    FJ_X11_ATOM_LIST_ITEM(UTF8_STRING)                  \
    FJ_X11_ATOM_LIST_ITEM(WM_DELETE_WINDOW)             \
    FJ_X11_ATOM_LIST_ITEM(WM_PROTOCOLS)

/** This makes a name for an enumeration member in fj_x11_atom that corresponds
 * to the atom. */
#define FJ_X11_ATOM_ID(X) FJ_X11_ATOM_##X

#define FJ_X11_GET_ATOM(APP, ATOM_NAME) \
    ((APP)->_data->atoms[FJ_X11_ATOM_ID(ATOM_NAME)])

#define FJ_X11_ATOM_LIST_ITEM(X) FJ_X11_ATOM_ID(X),
enum fj_x11_atom
{
    FJ_X11_ATOM_LIST FJ_X11_ATOM_MAX
};
#undef FJ_X11_ATOM_LIST_ITEM


struct fj_app_private_data
{
    struct fj_unix_events events;
    bool should_stop;

    Display *display;
    xcb_connection_t *connection;
    xcb_atom_t atoms[FJ_X11_ATOM_MAX];

    // struct fj_window_service window_service;
};


char const *fj_x11_error_into_string(uint8_t error_code);
char const *fj_x11_xcb_error_into_string(xcb_generic_error_t *error);

#endif
