#include <fejix/ext/helper.h>

#include <fejix/core/utils.h>

#include <stdlib.h>


static
char const */*[]*/const error_descriptions[] = {
    /* Ensures that this array contains enough elements (uninitialised elements will be NULL) */
    [FJ_ERR_MAX] = "...max error ID...",

    [FJ_OK] = "success",

    [FJ_ERR_UNKNOWN] = "unknown error",
    [FJ_ERR_ALLOCATION_FAILED] = "allocation failed (out of memory)",
    [FJ_ERR_INVALID_ALLOCATION] = "invalid allocation",
    [FJ_ERR_CONNECTION_FAILED] = "connection failed",
    [FJ_ERR_REQUEST_SENDING_FAILED] = "request sending failed",
    [FJ_ERR_REQUEST_FAILED] = "request failed",
    [FJ_ERR_EVENT_WAITING_FAILED] = "event waiting failed",
    [FJ_ERR_EVENT_READING_FAILED] = "event reading failed",
    [FJ_ERR_INVALID_TEXT_ENCODING] = "invalid text encoding",
    [FJ_ERR_INTERFACE_INIT_FAILED] = "interface initialisation failed",
    [FJ_ERR_TEMP_FILE_CREATION_FAILED] = "temporary file creation failed",
    [FJ_ERR_SHARED_MEMORY_ALLOCATION_FAILED] = "shared memory allocation failed",
};

static
char const */*[]*/const impl_names[] = {
    [FJ_IMPLEMENTATION_MAX] = "...max implementation ID...",

    [FJ_IMPLEMENTATION_ANDK] = "andk",
    [FJ_IMPLEMENTATION_APPKIT] = "appkit",
    [FJ_IMPLEMENTATION_WAYLAND] = "wayland",
    [FJ_IMPLEMENTATION_WINAPI] = "winapi",
    [FJ_IMPLEMENTATION_X11] = "x11",
};


char const */*[]*/ fj_ext_get_error_description(fj_err_t error)
{
    if (error >= FJ_ERR_USER) {
        return "user-defined error";
    }

    if (error >= FJ_ERR_MAX) {
        return "invalid error ID";
    }

    if (error_descriptions[error] == NULL) {
        return "some error (description not implemented yet)";
    };

    return error_descriptions[error];
}


char const */*[]?*/ fj_ext_get_implementation_name(fj_implementation_id_t impl_id)
{
    if (impl_id >= FJ_IMPLEMENTATION_MAX) {
        return NULL;
    }

    if (impl_names[impl_id] == NULL) {
        return "some implementation (name not implemented yet)";
    }

    return impl_names[impl_id];
}


char const */*[]?*/ fj_ext_get_implementation_hint(void)
{
    char const */*[]*/ hint;

    hint = getenv("FEJIX_IMPLEMENTATION");

    if (hint != NULL) {
        return hint;
    }

#if defined(FJ_OPT_WAYLAND) || defined(FJ_OPT_X11)
    hint = getenv("XDG_SESSION_TYPE");

    if (fj_streq(hint, "wayland") || fj_streq(hint, "x11")) {
        return hint;
    }
#endif

    return NULL;
}


struct fj_implementation_iface const */*?*/ fj_ext_choose_implementation(
    struct fj_implementation_iface const *const */*[]?*/ impls,
    uint32_t impl_count,
    char const */*[]?*/ impl_hint
)
{
    if (impl_count == 0) {
        return NULL;
    }

    if (impl_count == 1) {
        return impls[0];
    }

    for (uint32_t i=0; i<impl_count; i++) {
        char const * impl_name = fj_ext_get_implementation_name(impls[i]->id);

        if (fj_str_eq(impl_name, impl_hint)) {
            return impls[i];
        }
    }

    return NULL;
}
