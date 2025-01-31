#ifndef FEJIX_APP_MANUAL_SLEEP_H_
#define FEJIX_APP_MANUAL_SLEEP_H_


#include <fejix/interface/app.h>


/** TODO: docs */
FJ_PUBLIC
bool fj_app_manual_sleep_get_supported(void);


/**
    TODO: docs: how to siulate iterative event loop:
    on WinAPI, Wayland, X11: just call this
    on UIKit: call this from inside the first on_idle()
    on other: this interface is not possible to implement
*/
FJ_PUBLIC
fj_err_t fj_app_manual_sleep(struct fj_app *app);


#endif
