#include <fejix/ext/helpers/error.h>


/** Contains strings and NULLs for unimplemented descriptions. */
static char const *const error_descriptions[FJ_ERR_MAX + 1] = {
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


char const *fj_ext_error_get_description(fj_err_t error)
{
    if (error >= FJ_ERR_USER) {
        return "user-defined error";
    }

    if (error > FJ_ERR_MAX) {
        return "invalid error ID";
    }

    if (error_descriptions[error] == NULL) {
        return "some error (description not implemented yet)";
    };

    return error_descriptions[error];
}
