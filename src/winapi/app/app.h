#ifndef FEJIX_WINAPI_APP_INCLUDED
#define FEJIX_WINAPI_APP_INCLUDED


#include <fejix/app.h>

#include <windows.h>


struct fj_app {
    union fj_tag tag;
    struct fj_app_callbacks const *callbacks;

    HWND global_window;
    fj_seconds wakeup_timeout;
    fj_bool8 is_finished;
    fj_bool8 is_critical;
};


#endif
