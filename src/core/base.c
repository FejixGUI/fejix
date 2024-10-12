#include <fejix/core/base.h>


static uint32_t uint32_hash32(uint32_t x)
{
    x ^= x >> 16;
    x *= UINT32_C(0x45d9f3b);
    x ^= x >> 16;
    x *= UINT32_C(0x45d9f3b);
    x ^= x >> 16;
    return x;
}


static uint32_t uint64_hash32(uint64_t x)
{
    x ^= x >> 32;
    x *= UINT64_C(0xd6e8feb86659fd93);
    x ^= x >> 32;
    x *= UINT64_C(0xd6e8feb86659fd93);
    x ^= x >> 32;
    return (uint32_t) x;
}


static uint32_t uintptr_hash32(uintptr_t x)
{
    bool uintptr_fits_in_uint32 = sizeof(uintptr_t) <= sizeof(uint32_t);

    if (uintptr_fits_in_uint32) {
        return uint32_hash32((uint32_t) x);
    } else {
        return uint64_hash32((uint64_t) x);
    }
}


static uint32_t ptr_hash32(void *x)
{
    return uintptr_hash32((uintptr_t) x);
}


fj_bool8_t fj_tag_eq(union fj_tag a, union fj_tag b, fj_tag_type_t type)
{
    switch (type) {
        case FJ_TAG_U32:
            return a.u32 == b.u32;
        case FJ_TAG_I32:
            return a.i32 == b.i32;
        case FJ_TAG_U64:
            return a.u64 == b.u64;
        case FJ_TAG_I64:
            return a.i64 == b.i64;
        case FJ_TAG_UPTR:
            return a.uptr == b.uptr;
        case FJ_TAG_IPTR:
            return a.iptr == b.iptr;
        case FJ_TAG_PTR:
            return a.ptr == b.ptr;
        default:
            return false;
    }
}


uint32_t fj_tag_hash32(union fj_tag x, fj_tag_type_t type)
{
    switch (type) {
        case FJ_TAG_U32:
            return uint32_hash32(x.u32);
        case FJ_TAG_I32:
            return uint32_hash32((uint32_t) x.i32);
        case FJ_TAG_U64:
            return uint64_hash32(x.u64);
        case FJ_TAG_I64:
            return uint64_hash32((uint64_t) x.i64);
        case FJ_TAG_UPTR:
            return uintptr_hash32(x.uptr);
        case FJ_TAG_IPTR:
            return uintptr_hash32((uint64_t) x.iptr);
        case FJ_TAG_PTR:
            return ptr_hash32(x.ptr);

        default:
            return false;
    }
}
