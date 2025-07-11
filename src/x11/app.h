#ifndef FEJIX_X11_APP_H_
#define FEJIX_X11_APP_H_

#include <src/x11/x11.h>

#include <src/shared/common/macros.h>
#include <src/shared/unix/events.h>

#include <fejix/app.h>


// The atoms are sorted alphabetically here.

// clang-format off
#define FJ_X11_ATOM_MACROLIST \
    FJ_MACROLIST( \
        FJ_MACROITEM(_NET_WM_NAME) \
        FJ_MACROITEM(_NET_WM_SYNC_REQUEST) \
        FJ_MACROITEM(_NET_WM_SYNC_REQUEST_COUNTER) \
        FJ_MACROITEM(UTF8_STRING) \
        FJ_MACROITEM(WM_DELETE_WINDOW) \
        FJ_MACROITEM(WM_PROTOCOLS))
// clang-format on

#undef FJ_MACROLIST
#undef FJ_MACROITEM
#define FJ_MACROITEM(ITEM) FJ_X11_ATOM_##ITEM,
#define FJ_MACROLIST(...)           \
    enum fj_x11_atom                \
    {                               \
        __VA_ARGS__ FJ_X11_ATOM_MAX \
    };
FJ_X11_ATOM_MACROLIST

#define FJ_X11_GET_ATOM(APP, ATOM_NAME) \
    ((APP)->data->atoms[FJ_X11_ATOM_##ATOM_NAME])

struct fj_app_private_data
{
    struct fj_unix_events events;
    Display *display;
    xcb_connection_t *connection;
    xcb_atom_t atoms[FJ_X11_ATOM_MAX];

    // struct fj_window_service window_service;
};


#endif
