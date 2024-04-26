#ifndef FEJIX_HASH_H_
#define FEJIX_HASH_H_


#include <fejix/base.h>


uint32_t fj_uint32_hash32(uint32_t x);

uint32_t fj_uint64_hash32(uint64_t x);

/** Requires that pointers support clear conversions to `uintptr_t`, that is,
    if `a_ptr == b_ptr`, then `(uintptr_t) a_ptr == (uintptr_t) b_ptr`,
    which is in fact not guaranteed by the C99 standard, however works on most
    target platforms. */
uint32_t fj_uintptr_hash32(uintptr_t x);


#endif
