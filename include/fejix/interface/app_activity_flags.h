#ifndef FEJIX_APP_ACTIVITY_FLAGS_H_
#define FEJIX_APP_ACTIVITY_FLAGS_H_


#include <fejix/interface/app.h>


typedef uint32_t fj_app_activity_flags_t;

enum fj_app_activity_flags {
    /**
        The app will not be forced to finish while it is doing a critical activity.

        WinAPI: this prevents the system from logging off or shutting down.
    */
    FJ_APP_ACTIVITY_CRITICAL = 1 << 0,
};


FJ_PUBLIC
fj_app_activity_flags_t fj_app_activity_flags_get_supported_flags(struct fj_app *app);


FJ_PUBLIC
fj_err_t fj_app_activity_flags_set_flags(struct fj_app *app, fj_app_activity_flags_t flags);


#endif
