#ifndef FEJIX_APP_ACTIVITY_FLAGS_H_
#define FEJIX_APP_ACTIVITY_FLAGS_H_


#include <fejix/app.h>


typedef uint32_t fj_app_activity_flags_t;

enum fj_app_activity_flags {
    /**
        The app will not be forced to finish while it is doing a critical activity.

        WinAPI: this prevents the system from logging off or shutting down.
    */
    FJ_APP_ACTIVITY_CRITICAL = 1 << 0,
};


struct fj_app_activity_flags_funcs {
    fj_app_activity_flags_t (*get_supported_flags)(struct fj_app *app);

    fj_err_t (*set_flags)(struct fj_app *app, fj_app_activity_flags_t flags);
};


#endif
