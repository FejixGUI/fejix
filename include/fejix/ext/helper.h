#ifndef FEJIX_EXT_HELPER_H_
#define FEJIX_EXT_HELPER_H_


#include <fejix/implementation.h>


/** Returns implementation hint deduced from environment variables and/or compile options.
    Returns NULL if there are no environment hints. */
uint8_t const */*[]?*/ fj_ext_get_implementation_hint(void);

/** No implementation => returns NULL
    One implementation => returns it
    Many implementations + hint => finds the implementation (or NULL if not found).
    Many implementations + no hint => returns NULL. */
struct fj_implementation const */*?*/ fj_ext_choose_implementation(
    struct fj_implementation const *const */*[]?*/ implementations,
    uint32_t implementation_count,
    uint8_t const */*[]?*/ implementation_hint
);


#endif
