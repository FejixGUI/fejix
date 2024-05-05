#ifndef FEJIX_EXT_H_
#define FEJIX_EXT_H_


#include <fejix/implementation.h>
#include <fejix/interface/server.h>


uint8_t const *fjARRAY fj_ext_get_error_description(fj_err_t error);

/** Returns the implementation name corresponding to the given ID.
    Returns NULL for non-existing IDs */
uint8_t const *fjARRAY_OPTION fj_ext_get_implementation_name(
    fj_enum32_t implementation_id
);

/** Deduces which implementation the program should use from the environment.
    If there is only one implementation, returns its name.
    Returns NULL if deduction is impossible. */
uint8_t const *fjARRAY_OPTION fj_ext_get_implementation_hint(
    uint32_t implementation_count,
    struct fj_implementation const * const *fjARRAY implementations
);

struct fj_implementation const *fjOPTION fj_ext_find_implementation(
    uint8_t const *fjARRAY implementation_name,
    uint32_t implementation_count,
    struct fj_implementation const * const *fjARRAY implementations
);


#endif
