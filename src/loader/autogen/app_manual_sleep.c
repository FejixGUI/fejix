/* Automatically generated by tools/autogen.py, do not edit! */

#include <src/loader/loader.h>

#include <fejix/interface/app_manual_sleep.h>

fj_loader_function_t _fj_has_app_manual_sleep_ptr;
bool fj_has_app_manual_sleep(void)
{
    if (_fj_has_app_manual_sleep_ptr != NULL) {
        return ((bool(*)(void))_fj_has_app_manual_sleep_ptr)();
    }
    return false;
}

fj_loader_function_t _fj_app_manual_sleep_ptr;
fj_err_t fj_app_manual_sleep(struct fj_app * app)
{
    if (_fj_app_manual_sleep_ptr != NULL) {
        return ((fj_err_t(*)(struct fj_app * app))_fj_app_manual_sleep_ptr)(app);
    }
    return FJ_ERR_UNIMPLEMENTED;
}

