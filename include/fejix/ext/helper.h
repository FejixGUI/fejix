#ifndef FEJIX_EXT_HELPER_H_
#define FEJIX_EXT_HELPER_H_


#include <fejix/implementation.h>


/** Deduces which implementation the program should use from the environment.
    If there is only one implementation, returns its name.
    Returns NULL if deduction is impossible. */
uint8_t const */*[]?*/ fj_ext_get_implementation_hint(
    struct fj_implementation const *const */*[]*/ implementations,
    uint32_t implementation_count
);

struct fj_implementation const */*?*/ fj_ext_find_implementation(
    struct fj_implementation const *const */*[]*/ implementations,
    uint32_t implementation_count,
    uint8_t const */*[]*/ implementation_name
);


#endif
