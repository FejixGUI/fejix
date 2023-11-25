#include <fejix/protocol.h>
#include <fejix/utils.h>


#define LINK_PROTOCOL(NAME) extern struct fj_protocol const NAME;
#define USE_PROTOCOL(NAME) &NAME,


#ifdef FJ_OPT_WINAPI
    LINK_PROTOCOL(fj_winapi_protocol)
#endif
#ifdef FJ_OPT_X11
    LINK_PROTOCOL(fj_x11_protocol)
#endif

static struct fj_protocol const * const _protocols[] = {

#ifdef FJ_OPT_WINAPI
    USE_PROTOCOL(fj_winapi_protocol)
#endif
#ifdef FJ_OPT_X11
    USE_PROTOCOL(fj_x11_protocol)
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
