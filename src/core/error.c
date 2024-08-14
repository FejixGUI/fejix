#include <fejix/core/error.h>


static
uint8_t const */*[]*/const error_descriptions[] = {
    /* When a new error code gets added to the fj_error enum, but its
        description does not get added here, `[FJ_ERR_MAX]=` ensures that
        `error_descriptions[error_code]` returns NULL, which is handled
        by fj_get_error_description(). */
    [FJ_ERR_MAX] = FJ_UTF8("...max error ID..."),

    [FJ_OK] = FJ_UTF8("success"),

    [FJ_ERR_UNKNOWN] = FJ_UTF8("unknown error"),
    [FJ_ERR_OUT_OF_MEMORY] = FJ_UTF8("out of memory"),
    [FJ_ERR_INVALID_ALLOCATION] = FJ_UTF8("invalid memory allocation"),
    [FJ_ERR_IO_ERROR] = FJ_UTF8("IO error"),
    [FJ_ERR_MESSAGE_READ_ERROR] = FJ_UTF8("message read error"),
    [FJ_ERR_MESSAGE_SEND_ERROR] = FJ_UTF8("message send error"),
    [FJ_ERR_SHELL_CONNECTION_ERROR] = FJ_UTF8("shell connection error"),
    [FJ_ERR_INVALID_TEXT_ENCODING] = FJ_UTF8("invalid text encoding"),
};


uint8_t const */*[]*/ fj_get_error_description(fj_err_t error)
{
    if (error >= FJ_ERR_MAX) {
        return FJ_UTF8("invalid error ID");
    }

    if (error_descriptions[error] == NULL) {
        return FJ_UTF8("some error (description not implemented yet)");
    };

    return error_descriptions[error];
}
