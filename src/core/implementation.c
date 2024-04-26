#include <fejix/implementation.h>
#include <fejix/utils.h>


#ifdef FJ_OPT_NOOP
    extern struct fj_implementation const fj_noop_implementation;
#endif
#ifdef FJ_OPT_ANDK
    extern struct fj_implementation const fj_andk_implementation;
#endif
#ifdef FJ_OPT_COCOA
    extern struct fj_implementation const fj_cocoa_implementation;
#endif
#ifdef FJ_OPT_UIKIT
    extern struct fj_implementation const fj_uikit_implementation;
#endif
#ifdef FJ_OPT_WAYLAND
    extern struct fj_implementation const fj_wayland_implementation;
#endif
#ifdef FJ_OPT_WINAPI
    extern struct fj_implementation const fj_winapi_implementation;
#endif
#ifdef FJ_OPT_X11
    extern struct fj_implementation const fj_x11_implementation;
#endif


static
struct fj_implementation const * const implementation_list[] = {

#ifdef FJ_OPT_NOOP
    &fj_noop_implementation,
#endif
#ifdef FJ_OPT_ANDK
    &fj_andk_implementation,
#endif
#ifdef FJ_OPT_COCOA
    &fj_cocoa_implementation,
#endif
#ifdef FJ_OPT_UIKIT
    &fj_uikit_implementation,
#endif
#ifdef FJ_OPT_WAYLAND
    &fj_wayland_implementation,
#endif
#ifdef FJ_OPT_WINAPI
    &fj_winapi_implementation,
#endif
#ifdef FJ_OPT_X11
    &fj_x11_implementation,
#endif

};


void fj_get_imples(
    uint32_t fjOUT * implementation_count,
    struct fj_implementation const * const *fjARRAY fjOUT * implementations
)
{
    *implementation_count = FJ_ARRAY_LEN(implementation_list);
    *implementations = implementation_list;
}
