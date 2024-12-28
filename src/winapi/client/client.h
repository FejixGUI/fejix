#ifndef FEJIX_WINAPI_CLIENT_H_
#define FEJIX_WINAPI_CLIENT_H_


#include <fejix/interface/client.h>

#include <windows.h>


struct fj_client {
    FJ_EXTENDS_ABSTRACT_OBJECT

    HINSTANCE instance;

    HWND global_window;
    fj_seconds_t wakeup_timeout;
    fj_bool8_t is_finished;
};


#endif
