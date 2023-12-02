#ifndef FJ_WINAPI_PROTOCOL_H_
#define FJ_WINAPI_PROTOCOL_H_


#include <fejix/base.h>

#include <windows.h>


#define FJ_WINAPI_WINDOW_CLASS_DEFAULT (L"FEJIX_WINDOW_CLASS_DEFAULT")

#define FJ_WINAPI_STATE(STATE) ((struct fj_winapi_state *)(STATE))

struct fj_winapi_state {
    void * FJ_NULLABLE callback_data;

    HINSTANCE instance;
};


#endif
