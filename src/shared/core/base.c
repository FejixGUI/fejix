#include <src/shared/utils/memory.h>

#include <fejix/base.h>

#include <stdlib.h>
#include <string.h>


/** Each element is a string or NULL if unimplemented. */
static char const *error_strings[FJ_STATUS_MAX] = {
    [FJ_OK] = "success",
    [FJ_OUT_OF_MEMORY] = "out of memory",
    [FJ_UNIMPLEMENTED] = "unimplemented",
    [FJ_OPERATION_FAILED] = "operation failed",
    [FJ_IO_FAILED] = "input/output failed",
    [FJ_UNAVAILABLE] = "feature unavailable on the system",
    [FJ_ACCESS_DENIED] = "access denied",
    [FJ_CONCURRENT_ACCESS] = "concurrent access not permitted",
    [FJ_INVALID_USAGE] = "invalid usage (bug)",
    [FJ_INVALID_OPERATION] = "invalid operation",
    [FJ_INVALID_ENCODING] = "invalid text encoding",
};


char const *fj_status_into_string(enum fj_status s)
{
    if (s >= FJ_STATUS_MAX) {
        return "invalid error ID";
    }

    if (error_strings[s] == NULL) {
        return "unnamed error (did someone forget to write a description?)";
    }

    return error_strings[s];
}
