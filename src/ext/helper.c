#include <fejix/ext/helper.h>

#include <fejix/core/utils.h>

#include <stdlib.h>


char const */*[]?*/ fj_ext_get_implementation_hint(void)
{
    char const */*[]*/ hint;

    hint = getenv("FEJIX_IMPLEMENTATION");

    if (hint != NULL) {
        return hint;
    }

#if defined(FJ_OPT_WAYLAND) || defined(FJ_OPT_X11)
    hint = getenv("XDG_SESSION_TYPE");

    if (fj_streq(hint, "wayland") || fj_streq(hint, "x11")) {
        return hint;
    }
#endif

    return NULL;
}


struct fj_implementation const */*?*/ fj_ext_choose_implementation(
    struct fj_implementation const *const */*[]?*/ impls,
    uint32_t impl_count,
    char const */*[]?*/ impl_hint
)
{
    if (impl_count == 0) {
        return NULL;
    }

    if (impl_count == 1) {
        return impls[0];
    }

    for (uint32_t i=0; i<impl_count; i++) {
        char const * impl_name = fj_get_implementation_name(impls[i]->id);

        if (fj_streq(impl_name, impl_hint)) {
            return impls[i];
        }
    }

    return NULL;
}
