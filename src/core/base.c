#include <fejix/core/base.h>


/** Contains strings and NULLs for unimplemented descriptions. */
static char const *const error_descriptions[FJ_ERR_PREDEFINED_COUNT] = {
    [FJ_OK] = "success",
    [FJ_ERR_UNSUPPORTED] = "feature unsupported",
    [FJ_ERR_INTERNAL_FAILURE] = "internal failure",
    [FJ_ERR_CANNOT_ALLOCATE] = "cannot allocate",
    [FJ_ERR_INVALID_ALLOCATION] = "invalid allocation",
    [FJ_ERR_CANNOT_OPEN_FILE] = "cannot open file",
    [FJ_ERR_VECTOR_EMPTY] = "vector is empty, cannot remove",
    [FJ_ERR_INVALID_INDEX] = "invalid index",
    [FJ_ERR_CONNECTION_FAILED] = "connection failed",
    [FJ_ERR_INPUT_FAILED] = "input operation failed",
    [FJ_ERR_OUTPUT_FAILED] = "output operation failed",
    [FJ_ERR_WAITING_FAILED] = "waiting failed",
    [FJ_ERR_REQUEST_REJECTED] = "request rejected",
    [FJ_ERR_INVALID_ENCODING] = "invalid text encoding",
    [FJ_ERR_CANNOT_CREATE_TEMPFILE] = "cannot create temporary file",
    [FJ_ERR_CANNOT_ALLOCATE_SHARED] = "cannot allocate shared memory",
};


char const *fj_err_get_description(fj_err_t error)
{
    if (error >= FJ_ERR_USER) {
        return "user-defined error";
    }

    if (error >= FJ_ERR_PREDEFINED_COUNT) {
        return "unknown future error (external implementation might have used a newer core base)";
    }

    if (error_descriptions[error] == NULL) {
        return "some error (description not implemented yet)";
    };

    return error_descriptions[error];
}
