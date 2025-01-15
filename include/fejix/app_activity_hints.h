#ifndef FEJIX_APP_ACTIVITY_HINTS_H_
#define FEJIX_APP_ACTIVITY_HINTS_H_


#include <fejix/app.h>


typedef uint32_t fj_app_activity_hint_t;

enum fj_app_activity_hint {
    /**
        The app will not be forced to finish while it is doing a critical activity.

        WinAPI: this prevents the system from logging off or shutting down.
    */
    FJ_APP_ACTIVITY_CRITICAL,
};


struct fj_app_activity_hints_funcs {
    fj_bool8_t (*get_hint_supported)(struct fj_app *app, fj_app_activity_hint_t hint);

    fj_err_t (*set_hint)(struct fj_app *app, fj_app_activity_hint_t hint, fj_bool8_t value);
};


#endif
