#ifndef FEJIX_BUILTIN_PROVIDERS_H_
#define FEJIX_BUILTIN_PROVIDERS_H_


#include <fejix/interface/provider.h>


/**
    :param providers: Returns a nullable array of providers, sorted by provider IDs.
    :param provider_count: Returns the provider count.
*/
FJ_API void fj_get_builtin_providers(
    struct fj_provider *const **out_providers,
    uint32_t *out_provider_count
);


#endif
