/* Automatically generated by tools/autogen.py, do not edit! */


#include <fejix/interface/app_manual_sleep.h>
#include <src/loader.h>
fj_loader_function_t _fj_app_manual_sleep_get_supported_ptr;
bool fj_app_manual_sleep_get_supported(void)
{
    if (_fj_app_manual_sleep_get_supported_ptr != NULL) {
        return ((bool(*)(void))_fj_app_manual_sleep_get_supported_ptr)();
    }
    return 0;
}

fj_loader_function_t _fj_app_manual_sleep_ptr;
fj_err_t fj_app_manual_sleep(struct fj_app * app)
{
    if (_fj_app_manual_sleep_ptr != NULL) {
        return ((fj_err_t(*)(struct fj_app * app))_fj_app_manual_sleep_ptr)(app);
    }
    return FJ_ERR_UNSUPPORTED;
}

