/* Automatically generated by tools/autogen.py, do not edit! */

#include <src/loader/loader.h>

#include <fejix/interface/app_activity.h>

fj_loader_function_t _fj_app_activity_get_interface_version_ptr;
fj_version_t fj_app_activity_get_interface_version(struct fj_app * app)
{
    if (_fj_app_activity_get_interface_version_ptr != NULL) {
        return ((fj_version_t(*)(struct fj_app * app))_fj_app_activity_get_interface_version_ptr)(app);
    }
    return 0;
}

fj_loader_function_t _fj_app_activity_set_flags_ptr;
fj_err_t fj_app_activity_set_flags(struct fj_app * app, fj_app_activity_flags_t flags)
{
    if (_fj_app_activity_set_flags_ptr != NULL) {
        return ((fj_err_t(*)(struct fj_app * app, fj_app_activity_flags_t flags))_fj_app_activity_set_flags_ptr)(app, flags);
    }
    return FJ_ERR_UNSUPPORTED;
}

