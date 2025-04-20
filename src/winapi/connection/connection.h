#ifndef FEJIX_WINAPI_CONNECTION_H_INCLUDED
#define FEJIX_WINAPI_CONNECTION_H_INCLUDED


#include <fejix/modules/connection.h>

#include <windows.h>


struct fj_connection {
    void *userdata;
    fj_connection_event_callback callback;

    HINSTANCE instance;
    HWND message_only_window;
};


#endif
