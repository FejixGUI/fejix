#include <fejix/ext/helper.h>

#include <fejix/core/utils.h>

#include <stdlib.h>


uint8_t const */*[]?*/ fj_ext_get_implementation_hint(void)
{
    uint8_t const */*[]*/ hint;

    hint = FJ_AS_UTF8(getenv("FEJIX_IMPLEMENTATION"));

    if (hint != NULL) {
        return hint;
    }

#if defined(FJ_OPT_WAYLAND) || defined(FJ_OPT_X11)
    hint = FJ_AS_UTF8(getenv("XDG_SESSION_TYPE"));

    if (fj_streq(hint, FJ_UTF8("wayland")) || fj_streq(hint, FJ_UTF8("x11"))) {
        return hint;
    }
#endif

    return NULL;
}


struct fj_implementation const */*?*/ fj_ext_choose_implementation(
    struct fj_implementation const *const */*[]?*/ impls,
    uint32_t impl_count,
    uint8_t const */*[]?*/ impl_hint
)
{
    if (impl_count == 0) {
        return NULL;
    }

    if (impl_count == 1) {
        return impls[0];
    }

    for (uint32_t i=0; i<impl_count; i++) {
        uint8_t const * impl_name = fj_get_implementation_name(impls[i]->id);

        if (fj_streq(impl_name, impl_hint)) {
            return impls[i];
        }
    }

    return NULL;
}
