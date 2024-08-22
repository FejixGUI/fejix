#include <fejix/builtin_implementations.h>

#include <fejix/core/utils.h>


extern struct fj_implementation_iface const fj_andk_implementation_impl;
extern struct fj_implementation_iface const fj_appkit_implementation_impl;
extern struct fj_implementation_iface const fj_wayland_implementation_impl;
extern struct fj_implementation_iface const fj_winapi_implementation_impl;
extern struct fj_implementation_iface const fj_x11_implementation_impl;

#ifdef FJ_OPT_ANY_IMPLEMENTATION

    static
    struct fj_implementation_iface const *const implementation_list[] = {

#   ifdef FJ_OPT_IMPLEMENTATION_ANDK
        &fj_andk_implementation_impl,
#   endif
#   ifdef FJ_OPT_IMPLEMENTATION_APPKIT
        &fj_appkit_implementation_impl,
#   endif
#   ifdef FJ_OPT_IMPLEMENTATION_WAYLAND
        &fj_wayland_implementation_impl,
#   endif
#   ifdef FJ_OPT_IMPLEMENTATION_WINAPI
        &fj_winapi_implementation_impl,
#   endif
#   ifdef FJ_OPT_IMPLEMENTATION_X11
        &fj_x11_implementation_impl,
#   endif

    };

#endif


void fj_get_builtin_implementations(
    struct fj_implementation_iface const *const */*[]? out*/ * implementations,
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