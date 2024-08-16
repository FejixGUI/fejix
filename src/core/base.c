#include <fejix/core/base.h>


static
char const */*[]*/const error_descriptions[] = {
    /* Ensures that this array contains enough elements (uninitialised elements will be NULL) */
    [FJ_ERR_MAX] = "...max error ID...",

    [FJ_OK] = "success",

    [FJ_ERR_UNKNOWN] = "unknown error",
    [FJ_ERR_OUT_OF_MEMORY] = "out of memory",
    [FJ_ERR_INVALID_ALLOCATION] = "invalid memory allocation",
    [FJ_ERR_IO_ERROR] = "IO error",
    [FJ_ERR_MESSAGE_READ_ERROR] = "message read error",
    [FJ_ERR_MESSAGE_SEND_ERROR] = "message send error",
    [FJ_ERR_SHELL_CONNECTION_ERROR] = "shell connection error",
    [FJ_ERR_INVALID_TEXT_ENCODING] = "invalid text encoding",
};


char const */*[]*/ fj_get_error_description(fj_err_t error)
{
    if (error >= FJ_ERR_MAX) {
        return "invalid error ID";
    }

    if (error_descriptions[error] == NULL) {
        return "some error (description not implemented yet)";
    };

    return error_descriptions[error];
}
