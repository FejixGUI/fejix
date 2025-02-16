#ifndef FEJIX_APP_ACTIVITY_INCLUDED
#define FEJIX_APP_ACTIVITY_INCLUDED


#include <fejix/interface/app.h>


typedef uint32_t fj_app_activity_flags_t;

enum {
    /**
        The app will not be forced to finish while it is doing a critical activity.

        WinAPI: this prevents the system from logging off or shutting down.
    */
    FJ_APP_ACTIVITY_CRITICAL = 1 << 0,
};


FJ_PUBLIC
fj_version_t fj_app_activity_get_interface_version(struct fj_app *app);


FJ_PUBLIC
fj_err_t fj_app_activity_set_flags(struct fj_app *app, fj_app_activity_flags_t flags);


#endif
