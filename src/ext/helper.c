#include <fejix/ext/helper.h>

#include <fejix/core/utils.h>

#include <stdlib.h>


uint8_t const */*[]?*/ fj_ext_get_implementation_hint(
    struct fj_implementation const *const */*[]*/ impls,
    uint32_t impl_count
)
{
    if (impl_count == 1) {
        return fj_get_implementation_name(impls[0]->implementation_id);
    }

    uint8_t const */*[]*/ hint;

    hint = FJ_UTF8(getenv("FEJIX_IMPLEMENTATION"));

    if (hint != NULL) {
        return hint;
    }

#if defined(FJ_OPT_WAYLAND) || defined(FJ_OPT_X11)
    hint = FJ_UTF8(getenv("XDG_SESSION_TYPE"));

    if (
        FJ_STRING_EQ(hint, FJ_UTF8("wayland"))
        || FJ_STRING_EQ(hint, FJ_UTF8("x11"))
    ) {
        return hint;
    }
#endif

    return NULL;
}


struct fj_implementation const */*?*/ fj_ext_find_implementation(
    struct fj_implementation const *const */*[]*/ impls,
    uint32_t impl_count,
    uint8_t const */*[]*/ implementation_name
)
{
    for (uint32_t i=0; i<impl_count; i++) {
        fj_enum32_t impl_id = impls[i]->implementation_id;
        uint8_t const * name = fj_get_implementation_name(impl_id);

        if (FJ_STRING_EQ(name, implementation_name)) {
            return impls[i];
        }
    }

    return NULL;
}
