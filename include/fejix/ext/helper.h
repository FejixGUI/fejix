#ifndef FEJIX_EXT_HELPER_H_
#define FEJIX_EXT_HELPER_H_


#include <fejix/interface/implementation.h>


char const */*[]*/ fj_ext_get_error_description(fj_err_t error);

/** Returns the implementation name corresponding to the given ID.
    Returns NULL for non-existing IDs */
char const */*[]?*/ fj_ext_get_implementation_name(fj_implementation_id_t id);

/** Returns implementation hint deduced from environment variables and/or compile options.
    Returns NULL if there are no environment hints. */
char const */*[]?*/ fj_ext_get_implementation_hint(void);

/** No implementation => returns NULL
    One implementation => returns it
    Many implementations + hint => finds the implementation (or NULL if not found).
    Many implementations + no hint => returns NULL. */
struct fj_implementation_iface const */*?*/ fj_ext_choose_implementation(
    struct fj_implementation_iface const *const */*[]?*/ implementations,
    uint32_t implementation_count,
    char const */*[]?*/ implementation_hint
);


#endif
