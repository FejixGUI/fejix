#ifndef FEJIX_WINAPI_APP_H_
#define FEJIX_WINAPI_APP_H_


#include <fejix/app.h>

#include <windows.h>


struct fj_app {
    union fj_tag tag;
    struct fj_app_callbacks const *callbacks;

    HWND global_window;
    fj_seconds_t wakeup_timeout;
    fj_bool8_t is_finished;
    fj_bool8_t is_critical;
};


#endif
