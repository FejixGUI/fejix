#include <src/shared/utils/memory.h>

#include <fejix/base.h>

#include <stdlib.h>
#include <string.h>


/** Each element is a string or NULL if unimplemented. */
static char const *error_strings[FJ_ERROR_MAX] = {
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


char const *fj_error_into_string(enum fj_error e)
{
    if (e >= FJ_ERROR_MAX) {
        return "invalid error ID";
    }

    if (error_strings[e] == NULL) {
        return "unnamed error (did someone forget to write a description?)";
    }

    return error_strings[e];
}
