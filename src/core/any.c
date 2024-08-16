#include <fejix/core/any.h>


static
uint32_t uint32_hash32(uint32_t x)
{
    x ^= x >> 16;
    x *= UINT32_C(0x45d9f3b);
    x ^= x >> 16;
    x *= UINT32_C(0x45d9f3b);
    x ^= x >> 16;
    return x;
}


static
uint32_t uint64_hash32(uint64_t x)
{
    x ^= x >> 32;
    x *= UINT64_C(0xd6e8feb86659fd93);
    x ^= x >> 32;
    x *= UINT64_C(0xd6e8feb86659fd93);
    x ^= x >> 32;
    return (uint32_t) x;
}


fj_bool32_t fj_any_eq(union fj_any a, union fj_any b, fj_any_type_t type)
{
    switch (type) {
        case FJ_TYPE_U32:  return a.u32  == b.u32;
        case FJ_TYPE_I32:  return a.i32  == b.i32;
        case FJ_TYPE_U64:  return a.u64  == b.u64;
        case FJ_TYPE_I64:  return a.i64  == b.i64;
        default: return false;
    }
}


uint32_t fj_any_hash32(union fj_any x, fj_any_type_t type)
{
    switch (type) {
        case FJ_TYPE_U32:  return uint32_hash32(x.u32);
        case FJ_TYPE_I32:  return uint32_hash32((uint32_t) x.i32);
        case FJ_TYPE_U64:  return uint64_hash32(x.u64);
        case FJ_TYPE_I64:  return uint64_hash32((uint64_t) x.i64);

        default: return false;
    }
}
