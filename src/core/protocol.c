#include <fejix/protocol.h>
#include <fejix/utils.h>

#include <stdlib.h>


#define LINK_PROTOCOL(NAME) extern struct fj_protocol const NAME;
#define USE_PROTOCOL(NAME) &NAME,


#ifdef FJ_OPT_WINAPI
    LINK_PROTOCOL(fj_winapi_protocol)
#endif
#ifdef FJ_OPT_X11
    LINK_PROTOCOL(fj_x11_protocol)
#endif

static struct fj_protocol const * const protocol_list[] = {

#ifdef FJ_OPT_WINAPI
    USE_PROTOCOL(fj_winapi_protocol)
#endif
#ifdef FJ_OPT_X11
    USE_PROTOCOL(fj_x11_protocol)
#endif

};


fj_string_t fj_get_protocol_hint(void)
{
    fj_string_t protocol = NULL;

    protocol = (fj_string_t) getenv("FEJIX_PROTOCOL");

    if (protocol != NULL) {
        return protocol;
    }

    protocol = (fj_string_t) getenv("XDG_SESSION_TYPE");

    if (fj_str_eq(protocol, FJ_UTF8("x11"))
     || fj_str_eq(protocol, FJ_UTF8("wayland")))
    {
        return protocol;
    }

    return protocol_list[0]->name;
}


void fj_get_protocols(
    struct fj_protocol const * const * FJ_ARRAY FJ_OUT * protocols,
    uint32_t FJ_OUT * protocol_count
)
{
    *protocols = protocol_list;
    *protocol_count = FJ_ARRLEN(protocol_list);
}
