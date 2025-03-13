#include <fejix/core.h>
#include <fejix/utils/memory.h>


/** Each element is a string or NULL if unimplemented. */
static char const *error_descriptions[] = {
    [FJ_OK] = "success",
    [FJ_ERR_OUT_OF_MEMORY] = "out of memory",
    [FJ_ERR_UNIMPLEMENTED] = "unimplemented",
    [FJ_ERR_OPERATION_FAILED] = "operation failed",
    [FJ_ERR_IO_FAILED] = "input/output failed",
    [FJ_ERR_UNAVAILABLE] = "feature unavailable on the system",
    [FJ_ERR_ACCESS_DENIED] = "access denied",
    [FJ_ERR_CONCURRENT_ACCESS] = "concurrent access not permitted",
    [FJ_ERR_INVALID_USAGE] = "invalid usage (bug)",
    [FJ_ERR_INVALID_OPERATION] = "invalid operation",
    [FJ_ERR_INVALID_ENCODING] = "invalid text encoding",
};


char const *fj_err_get_description(fj_err_t error)
{
    if (error >= FJ_LEN(error_descriptions) || error_descriptions[error] == NULL) {
        return "unknown error (did someone forget to write a description?)";
    }

    return error_descriptions[error];
}
