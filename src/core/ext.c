#include <fejix/ext.h>
#include <fejix/utils.h>

#include <stdlib.h>
#include <stdio.h>


static
uint8_t const *fjARRAY error_descriptions[] = {
    /* When a new error code gets added to the fj_error enum, but its
        description does not get added here, `[FJ_ERR_MAX]=` ensures that
        `error_descriptions[error_code]` returns NULL, which is handled
        by fj_ext_get_error_description(). */
    [FJ_ERR_MAX] = FJ_UTF8("maximum error ID"),

    [FJ_OK] = FJ_UTF8("success"),

    [FJ_ERR_UNKNOWN] = FJ_UTF8("unknown error"),
    [FJ_ERR_OUT_OF_MEMORY] = FJ_UTF8("out of memory"),
    [FJ_ERR_MALLOC_INVALID_ARG] = FJ_UTF8("invalid memory allocation arg"),
};


uint8_t const *fjARRAY fj_ext_get_error_description(fj_err_t error)
{
    if (error > FJ_ERR_MAX) {
        return FJ_UTF8("invalid error ID");
    }

    if (error_descriptions[error] == NULL) {
        return FJ_UTF8("error (description not implemented yet)");
    };

    return error_descriptions[error];
}



static
uint8_t const *fjARRAY implementation_names[] = {
    [FJ_IMPLEMENTATION_NOOP] = FJ_UTF8("noop"),
    [FJ_IMPLEMENTATION_ANDK] = FJ_UTF8("andk"),
    [FJ_IMPLEMENTATION_COCOA] = FJ_UTF8("cocoa"),
    [FJ_IMPLEMENTATION_UIKIT] = FJ_UTF8("uikit"),
    [FJ_IMPLEMENTATION_WAYLAND] = FJ_UTF8("wayland"),
    [FJ_IMPLEMENTATION_WINAPI] = FJ_UTF8("winapi"),
    [FJ_IMPLEMENTATION_X11] = FJ_UTF8("x11"),
};


uint8_t const *fjARRAY_OPTION fj_ext_get_implementation_hint(void)
{
    uint32_t impl_count;
    struct fj_implementation const * const *fjARRAY impls;

    fj_get_builtin_implementations(&impl_count, &impls);

    if (impl_count == 1) {
        return fj_ext_get_implementation_name(impls[0]->implementation_id);
    }

    uint8_t const *fjARRAY hint;

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


uint8_t const *fjARRAY_OPTION fj_ext_get_implementation_name(
    fj_enum32_t implementation_id
)
{
    return implementation_names[implementation_id];
}


struct fj_implementation const *fjOPTION fj_ext_find_builtin_implementation(
    uint8_t const *fjARRAY implementation_name
)
{
    uint32_t impl_count;
    struct fj_implementation const * const *fjARRAY impls;

    fj_get_builtin_implementations(&impl_count, &impls);

    for (uint32_t i=0; i<impl_count; i++) {
        fj_enum32_t impl_id = impls[i]->implementation_id;
        uint8_t const * name = fj_ext_get_implementation_name(impl_id);

        if (FJ_STRING_EQ(name, implementation_name)) {
            return impls[i];
        }
    }

    return NULL;
}
