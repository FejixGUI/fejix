#ifndef FEJIX_ERROR_H_
#define FEJIX_ERROR_H_


#include <fejix/base.h>


#define FJ_INIT_TRY fj_err_t _fj_err = FJ_OK;
#define FJ_TRY _fj_err =
#define FJ_ELSE if (_fj_err != FJ_OK)
#define FJ_RESULT (_fj_err)


enum fj_err {
    FJ_OK = 0,

    FJ_ERR_UNKNOWN,
    FJ_ERR_INTERFACE_UNSUPPORTED,
    FJ_ERR_OUT_OF_MEMORY,
    FJ_ERR_MALLOC_INVALID_ARG,
    FJ_ERR_IO_ERROR,
    FJ_ERR_MESSAGE_RECEIVE_ERROR,

    FJ_ERR_MAX,
};


#endif
