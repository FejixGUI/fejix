#include <fejix/core.h>
#include <fejix/utils/memory.h>


/** Each element is a string or NULL if unimplemented. */
static char const *error_descriptions[] = {
    [FJ_OK] = "success",
    [FJ_ERROR_OUT_OF_MEMORY] = "out of memory",
    [FJ_ERROR_UNIMPLEMENTED] = "unimplemented",
    [FJ_ERROR_OPERATION_FAILED] = "operation failed",
    [FJ_ERROR_IO_FAILED] = "input/output failed",
    [FJ_ERROR_UNAVAILABLE] = "feature unavailable on the system",
    [FJ_ERROR_ACCESS_DENIED] = "access denied",
    [FJ_ERROR_CONCURRENT_ACCESS] = "concurrent access not permitted",
    [FJ_ERROR_INVALID_USAGE] = "invalid usage (bug)",
    [FJ_ERROR_INVALID_OPERATION] = "invalid operation",
    [FJ_ERROR_INVALID_ENCODING] = "invalid text encoding",
};


char const *fj_error_get_description(enum fj_error error)
{
    if (error >= FJ_LEN(error_descriptions) || error_descriptions[error] == NULL) {
        return "unknown error (did someone forget to write a description?)";
    }

    return error_descriptions[error];
}
