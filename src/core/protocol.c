#include <fejix/protocol.h>
#include <fejix/utils.h>


#ifdef FJ_OPT_ANDK
    extern struct fj_protocol const fj_andk_protocol;
#endif
#ifdef FJ_OPT_COCOA
    extern struct fj_protocol const fj_cocoa_protocol;
#endif
#ifdef FJ_OPT_UIKIT
    extern struct fj_protocol const fj_uikit_protocol;
#endif
#ifdef FJ_OPT_WAYLAND
    extern struct fj_protocol const fj_wayland_protocol;
#endif
#ifdef FJ_OPT_WINAPI
    extern struct fj_protocol const fj_winapi_protocol;
#endif
#ifdef FJ_OPT_X11
    extern struct fj_protocol const fj_x11_protocol;
#endif


static
struct fj_protocol const * const _protocols[] = {

#ifdef FJ_OPT_ANDK
    &fj_andk_protocol,
#endif
#ifdef FJ_OPT_COCOA
    &fj_cocoa_protocol,
#endif
#ifdef FJ_OPT_UIKIT
    &fj_uikit_protocol,
#endif
#ifdef FJ_OPT_WAYLAND
    &fj_wayland_protocol,
#endif
#ifdef FJ_OPT_WINAPI
    &fj_winapi_protocol,
#endif
#ifdef FJ_OPT_X11
    &fj_x11_protocol,
#endif

};


void fj_get_protocols(
    struct fj_protocol const * const * FJ_ARRAY FJ_OUT * protocols,
    uint32_t FJ_OUT * protocol_count
)
{
    *protocols = _protocols;
    *protocol_count = FJ_ARRLEN(_protocols);
}
