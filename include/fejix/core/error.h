#ifndef FEJIX_CORE_ERROR_H_
#define FEJIX_CORE_ERROR_H_


#include <fejix/core/base.h>

#define FJ_INIT_TRY fj_err_t _fj_err = FJ_OK;
#define fj_try _fj_err =
#define fj_else if (_fj_err != FJ_OK)
#define fj_result (_fj_err)


enum fj_err {
    FJ_OK = 0,

    FJ_ERR_UNKNOWN,
    FJ_ERR_INTERFACE_UNSUPPORTED,
    FJ_ERR_OUT_OF_MEMORY,
    FJ_ERR_INVALID_ALLOCATION,
    FJ_ERR_IO_ERROR,
    FJ_ERR_MESSAGE_READ_ERROR,
    FJ_ERR_SHELL_CONNECTION_ERROR,

    FJ_ERR_MAX,
};


uint8_t const */*[]*/ fj_get_error_description(
    fj_err_t error
);


#endif
