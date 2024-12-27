#include <fejix/builtin_providers.h>

#include <fejix/ext/helpers/provider.h>

#include <fejix/core/utils.h>

#include <stdlib.h>


/** Contains strings and NULLs for unimplemented names. */
static char const *const provider_names[FJ_PROVIDER_MAX + 1] = {
    [FJ_PROVIDER_ANDK] = "andk",       [FJ_PROVIDER_COCOA] = "cocoa",
    [FJ_PROVIDER_WAYLAND] = "wayland", [FJ_PROVIDER_WINAPI] = "winapi",
    [FJ_PROVIDER_X11] = "x11",
};


char const *fj_ext_provider_get_name(fj_provider_id_t provider_id)
{
    if (provider_id > FJ_PROVIDER_MAX) {
        return NULL;
    }

    if (provider_names[provider_id] == NULL) {
        return "some provider (name not implemented yet)";
    }

    return provider_names[provider_id];
}


char const *fj_ext_provider_get_hint(void)
{
    char const *hint;

    hint = getenv("FEJIX_PROVIDER");

    if (hint != NULL) {
        return hint;
    }

#if defined(FJ_OPT_WAYLAND) || defined(FJ_OPT_X11)
    hint = getenv("XDG_SESSION_TYPE");

    if (fj_str_eq(hint, "wayland") || fj_str_eq(hint, "x11")) {
        return hint;
    }

    if (getenv("WAYLAND_DISPLAY") != NULL) {
        return "wayland";
    }

    if (getenv("DISPLAY") != NULL) {
        return "x11";
    }
#endif

    return NULL;
}


struct fj_provider *fj_ext_provider_choose_hinted(
    struct fj_provider *const *providers,
    uint32_t provider_count,
    char const *provider_hint
)
{
    if (provider_count == 0) {
        return NULL;
    }

    if (provider_count == 1) {
        return providers[0];
    }

    for (uint32_t i = 0; i < provider_count; i++) {
        fj_provider_id_t provider_id;
        FJ_TRY (FJ_RESPOND(providers[i], FJ_PROVIDER_GET_ID, NULL, &provider_id)) {
            continue;
        }

        char const *provider_name = fj_ext_provider_get_name(provider_id);

        if (fj_str_eq(provider_name, provider_hint)) {
            return providers[i];
        }
    }

    return NULL;
}


struct fj_provider *fj_ext_provider_choose_default(void)
{
    struct fj_provider *const *providers;
    uint32_t provider_count;
    fj_get_builtin_providers(&providers, &provider_count);
    return fj_ext_provider_choose_hinted(providers, provider_count, fj_ext_provider_get_hint());
}
