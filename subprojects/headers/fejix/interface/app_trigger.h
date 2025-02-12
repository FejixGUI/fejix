#ifndef FEJIX_APP_TRIGGER_H_
#define FEJIX_APP_TRIGGER_H_


#include <fejix/interface/app.h>


FJ_PUBLIC
fj_version_t fj_app_trigger_get_interface_version(void);

/**
    Immediately makes an app that is waiting for events wake up from another thread.

    Implementation-defined behavior: after calling this on the main thread, the next time when
    trying to sleep the app can immediately wake up, thus clearing the wakeup timeout.
    To handle this, the app should always set a relevant wakeup timeout when pausing.
*/
FJ_PUBLIC
fj_err_t fj_app_trigger(struct fj_app *app);


#endif
