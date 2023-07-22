#ifndef FEJIX_TYPES_H_
#define FEJIX_TYPES_H_


#include <stdint.h>


typedef uint32_t fj_result_t;
typedef uint32_t fj_id_t;
typedef uint32_t fj_bool_t;
typedef void * fj_ptr_t;
typedef char * fj_string_t;


enum fj_result_values {
    FJ_OK = 0,
    FJ_ALLOCATION_FAILED = 1,
    FJ_INTERNAL_ERROR = 2,
    FJ_PLATFORM_API_FAILED = 3,
    FJ_GRAPHICS_API_FAILED = 4,
};

enum fj_bool_values {
    FJ_FALSE = 0,
    FJ_TRUE = 1,
};


#endif