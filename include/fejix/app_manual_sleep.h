#ifndef FEJIX_APP_MANUAL_SLEEP_H_
#define FEJIX_APP_MANUAL_SLEEP_H_


#include <fejix/app.h>


struct fj_app_manual_sleep_funcs {
    fj_err_t (*manual_sleep)(struct fj_app *app);
};


#endif
