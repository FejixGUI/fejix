#include <fejix/utils.h>


uint32_t fj_uint32_hash32(uint32_t x)
{
    x ^= x >> 16;
    x *= UINT32_C(0x45d9f3b);
    x ^= x >> 16;
    x *= UINT32_C(0x45d9f3b);
    x ^= x >> 16;
    return x;
}

uint32_t fj_uint64_hash32(uint64_t x)
{
    x ^= x >> 32;
    x *= UINT64_C(0xd6e8feb86659fd93);
    x ^= x >> 32;
    x *= UINT64_C(0xd6e8feb86659fd93);
    x ^= x >> 32;
    return (uint32_t) x;
}

uint32_t fj_uintptr_hash32(uintptr_t x)
{
    if (sizeof(x) <= 4) {
        return fj_uint32_hash32((uint32_t) x);
    } else {
        return fj_uint64_hash32((uint64_t) x);
    }
}
