#ifndef FEJIX_APP_MANUAL_SLEEP_INCLUDED
#define FEJIX_APP_MANUAL_SLEEP_INCLUDED


#include <fejix/interface/app.h>


FJ_PUBLIC
bool fj_has_app_manual_sleep(void);


/**
    TODO: docs: how to simulate iterative event loop:
    on WinAPI, Wayland, X11: just call this
    on UIKit: call this from inside the first continue callback
    on other: this interface is not possible to implement
*/
FJ_PUBLIC
fj_err_t fj_app_manual_sleep(struct fj_app *app);


#endif
