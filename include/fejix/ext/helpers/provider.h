#ifndef FEJIX_EXT_HELPER_H_
#define FEJIX_EXT_HELPER_H_


#include <fejix/interface/provider.h>


/**
    Returns the provider name corresponding to the given ID.
    Returns NULL for non-existing IDs
*/
FJ_API
char const *fj_ext_provider_get_name(fj_provider_id_t id);

/**
    Returns provider hint deduced from environment variables and/or compile options.
    Returns NULL if there are no environment hints.
*/
FJ_API
char const *fj_ext_provider_get_hint(void);

/**
    No providers => returns NULL
    One provider => returns it
    Many providers + hint => finds the provider (or NULL if not found).
    Many providers + no hint => returns NULL.
*/
FJ_API
struct fj_provider *fj_ext_provider_choose_hinted(
    struct fj_provider *const *providers,
    uint32_t provider_count,
    char const *provider_hint
);

/**
    Chooses from builtin providers based on the default hint.

    :returns: The provider or NULL if no provider can be chosen.
*/
FJ_API
struct fj_provider *fj_ext_provider_choose_default(void);

#endif
