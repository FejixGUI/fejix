#ifndef FEJIX_ERROR_H_
#define FEJIX_ERROR_H_


#include <fejix/base.h>


#define FJ_INIT_TRY fj_err_t _fj_result = FJ_OK;
#define FJ_TRY _fj_result =
#define FJ_ELSE if (_fj_result != FJ_OK)
#define FJ_RESULT (_fj_result)


enum fj_err {
    FJ_OK = 0,
    FJ_ERR_UNKNOWN,
    FJ_ERR_OUT_OF_MEMORY,
    FJ_ERR_MALLOC_INVALID_ARG,
    FJ_ERR_MAX,
};


#endif
