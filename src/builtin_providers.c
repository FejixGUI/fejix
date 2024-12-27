#include <fejix/builtin_providers.h>

#include <fejix/core/utils.h>


extern struct fj_provider fj_andk_provider;
extern struct fj_provider fj_cocoa_provider;
extern struct fj_provider fj_wayland_provider;
extern struct fj_provider fj_winapi_provider;
extern struct fj_provider fj_x11_provider;

static struct fj_provider *const builtin_providers[] = {

#ifdef FJ_OPT_ANDK
    &fj_andk_provider,
#endif
#ifdef FJ_OPT_COCOA
    &fj_cocoa_provider,
#endif
#ifdef FJ_OPT_WAYLAND
    &fj_wayland_provider,
#endif
#ifdef FJ_OPT_WINAPI
    &fj_winapi_provider,
#endif
#ifdef FJ_OPT_X11
    &fj_x11_provider,
#endif

    NULL,  // This will keep the array non-empty even when no providers are enabled.

};


void fj_get_builtin_providers(
    struct fj_provider *const **out_providers,
    uint32_t *out_provider_count
)
{
    *out_provider_count = FJ_LEN(builtin_providers) - 1;
    *out_providers = *out_provider_count > 0 ? builtin_providers : NULL;
}
