#ifndef FEJIX_CORE_ANY_H_
#define FEJIX_CORE_ANY_H_


#include <fejix/core/base.h>


#define FJ_U32(X) ((union fj_any) { .u32 = (uint32_t) (X) })
#define FJ_I32(X) ((union fj_any) { .i32 = (int32_t)  (X) })
#define FJ_U64(X) ((union fj_any) { .u64 = (uint64_t) (X) })
#define FJ_I64(X) ((union fj_any) { .i64 = (int64_t)  (X) })


typedef uint32_t fj_any_type_t;

enum fj_any_type {
    FJ_TYPE_U32,
    FJ_TYPE_I32,
    FJ_TYPE_U64,
    FJ_TYPE_I64,
};


union fj_any {
    uint32_t u32;
    int32_t  i32;
    uint64_t u64;
    int64_t  i64;
};


fj_bool32_t fj_any_eq(union fj_any a, union fj_any b, fj_any_type_t type);

uint32_t fj_any_hash32(union fj_any x, fj_any_type_t type);


#endif
