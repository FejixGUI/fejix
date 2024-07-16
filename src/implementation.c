#include <fejix/implementation.h>

#include <fejix/core/utils.h>


static
uint8_t const */*[]*/ implementation_names[] = {
    [FJ_IMPLEMENTATION_MAX] = FJ_UTF8("...max implementation ID..."),

    [FJ_IMPLEMENTATION_ANDK] = FJ_UTF8("andk"),
    [FJ_IMPLEMENTATION_COCOA] = FJ_UTF8("cocoa"),
    [FJ_IMPLEMENTATION_WAYLAND] = FJ_UTF8("wayland"),
    [FJ_IMPLEMENTATION_WINAPI] = FJ_UTF8("winapi"),
    [FJ_IMPLEMENTATION_X11] = FJ_UTF8("x11"),
};


extern struct fj_implementation const fj_andk_implementation;
extern struct fj_implementation const fj_cocoa_implementation;
extern struct fj_implementation const fj_wayland_implementation;
extern struct fj_implementation const fj_winapi_implementation;
extern struct fj_implementation const fj_x11_implementation;

#ifdef FJ_OPT_ANY_IMPLEMENTATION

    static
    struct fj_implementation const *const implementation_list[] = {

#   ifdef FJ_OPT_ANDK
        &fj_andk_implementation,
#   endif
#   ifdef FJ_OPT_COCOA
        &fj_cocoa_implementation,
#   endif
#   ifdef FJ_OPT_WAYLAND
        &fj_wayland_implementation,
#   endif
#   ifdef FJ_OPT_WINAPI
        &fj_winapi_implementation,
#   endif
#   ifdef FJ_OPT_X11
        &fj_x11_implementation,
#   endif

    };

#endif


uint8_t const */*[]?*/ fj_get_implementation_name(
    fj_enum32_t implementation_id
)
{
    if (implementation_id >= FJ_IMPLEMENTATION_MAX) {
        return NULL;
    }

    if (implementation_names[implementation_id] == NULL) {
        return FJ_UTF8("some implementation (name not implemented yet)");
    }

    return implementation_names[implementation_id];
}


void fj_get_builtin_implementations(
    struct fj_implementation const *const */*[]? out*/ * implementations,
    uint32_t /*out*/ * implementation_count
)
{
#ifdef FJ_OPT_ANY_IMPLEMENTATION
    *implementations = implementation_list;
    *implementation_count = FJ_ARRAY_LEN(implementation_list);
#else
    *implementations = NULL;
    *implementation_count = 0;
#endif
}
