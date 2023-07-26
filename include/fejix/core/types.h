#ifndef FEJIX_TYPES_H_
#define FEJIX_TYPES_H_


#include <stdint.h>
#include <stddef.h>


typedef uint32_t fj_err_t;
typedef uint32_t fj_id_t;
typedef void * fj_ptr_t;

/// UTF-8 string
typedef char * fj_string_t;


enum fj_result_values {
    FJ_OK = 0,
    FJ_MALLOC_FAIL = 1,
    FJ_INTERNAL_FAIL = 2,
    FJ_SYSTEM_FAIL = 3,
    FJ_INVALID_ID = 4,
};

enum fj_bool_values {
    FJ_FALSE = 0,
    FJ_TRUE = 1,
};


#endif