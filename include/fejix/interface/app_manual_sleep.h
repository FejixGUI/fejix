#ifndef FJ_INTERFACE_APP_MANUAL_SLEEP_H_
#define FJ_INTERFACE_APP_MANUAL_SLEEP_H_


#include <fejix/interface/app.h>


struct fj_app_manual_sleep_class {
    fj_err_t (*manual_sleep)(struct fj_app *app);
};


#endif
