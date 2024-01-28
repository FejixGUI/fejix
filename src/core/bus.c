#include <fejix/bus.h>
#include <fejix/utils.h>


#ifdef FJ_OPT_ANDK
    extern struct fj_bus const fj_andk_bus;
#endif
#ifdef FJ_OPT_COCOA
    extern struct fj_bus const fj_cocoa_bus;
#endif
#ifdef FJ_OPT_UIKIT
    extern struct fj_bus const fj_uikit_bus;
#endif
#ifdef FJ_OPT_WAYLAND
    extern struct fj_bus const fj_wayland_bus;
#endif
#ifdef FJ_OPT_WINAPI
    extern struct fj_bus const fj_winapi_bus;
#endif
#ifdef FJ_OPT_X11
    extern struct fj_bus const fj_x11_bus;
#endif


static
struct fj_bus const * const bus_list[] = {

#ifdef FJ_OPT_ANDK
    &fj_andk_bus,
#endif
#ifdef FJ_OPT_COCOA
    &fj_cocoa_bus,
#endif
#ifdef FJ_OPT_UIKIT
    &fj_uikit_bus,
#endif
#ifdef FJ_OPT_WAYLAND
    &fj_wayland_bus,
#endif
#ifdef FJ_OPT_WINAPI
    &fj_winapi_bus,
#endif
#ifdef FJ_OPT_X11
    &fj_x11_bus,
#endif

};


void fj_get_buses(
    uint32_t FJ_OUT * bus_count,
    struct fj_bus const * const * FJ_ARRAY FJ_OUT * buses
)
{
    *bus_count = FJ_ARRLEN(bus_list);
    *buses = bus_list;
}


fj_err_t fj_bus_open(
    struct fj_bus const * bus,
    void * FJ_NULLABLE FJ_OUT * bus_context,
    fj_bus_listener_t * bus_listener
)
{
    return bus->open(bus_context, bus_listener);
}


void fj_bus_close(
    struct fj_bus const * bus,
    void * bus_context
)
{
    bus->close(bus_context);
}


void fj_bus_get_sockets(
    struct fj_bus const * bus,
    void * bus_context,
    uint32_t FJ_OUT * socket_count,
    struct fj_socket const * const * FJ_ARRAY FJ_OUT * sockets
)
{
    bus->get_sockets(bus_context, socket_count, sockets);
}


fj_err_t fj_bus_serve(
    struct fj_bus const * bus,
    void * bus_context,
    fj_serve_type_t serve_type,
    void * FJ_NULLABLE serve_data
)
{
    return bus->serve(bus_context, serve_type, serve_data);
}


fj_err_t fj_bus_send(
    struct fj_bus const * bus,
    void * bus_context,
    uint32_t message_count,
    struct fj_message const * FJ_ARRAY messages
)
{
    return bus->send(bus_context, message_count, messages);
}


fj_err_t fj_socket_open(
    struct fj_socket * socket,
    void * bus_context,
    void * FJ_NULLABLE FJ_OUT * socket_context
)
{
    return socket->open(bus_context, socket_context);
}


void fj_socket_close(
    void * socket_context
)
{
    fj_socket_context_get_socket(socket_context)->close(socket_context);
}


fj_err_t fj_socket_open_device(
    void * socket_context,
    void * device_handle,
    void * FJ_NULLABLE device_open_info,
    void * FJ_NULLABLE FJ_OUT * device_context
)
{
    return fj_socket_context_get_socket(socket_context)->open_device(
        socket_context,
        device_handle,
        device_open_info,
        device_context
    );
}


void fj_socket_close_device(
    void * device_context
)
{
    fj_device_context_get_socket(device_context)->close_device(device_context);
}


fj_bool_t fj_socket_supports(
    void * device_context,
    fj_message_id_t message_id
)
{
    return fj_device_context_get_socket(device_context)->supports(
        device_context,
        message_id
    );
}


struct fj_bus const * fj_bus_context_get_bus(
    void const * bus_context
)
{
    return ((struct fj_bus_context_base *) bus_context)->bus;
}


void * FJ_NULLABLE fj_bus_context_get_user_data(
    void const * bus_context
)
{
    return ((struct fj_bus_context_base *) bus_context)->user_data;
}


void fj_bus_context_set_user_data(
    void * bus_context,
    void * FJ_NULLABLE user_data
)
{
    ((struct fj_bus_context_base *) bus_context)->user_data = user_data;
}


struct fj_socket const * fj_socket_context_get_socket(
    void const * socket_context
)
{
    return ((struct fj_socket_context_base *) socket_context)->socket;
}


void * fj_socket_context_get_bus_context(
    void const * socket_context
)
{
    return ((struct fj_socket_context_base *) socket_context)->bus_context;
}


struct fj_bus const * fj_socket_context_get_bus(
    void const * socket_context
)
{
    return fj_bus_context_get_bus(
        fj_socket_context_get_bus_context(socket_context)
    );
}


void * FJ_NULLABLE fj_device_context_get_parent(
    void const * device_context
)
{
    return ((struct fj_device_context_base *) device_context)->parent_context;
}


void * fj_device_context_get_handle(
    void const * device_context
)
{
    return ((struct fj_device_context_base *) device_context)->device_handle;
}


void * fj_device_context_get_socket_context(
    void const * device_context
)
{
    return ((struct fj_device_context_base *) device_context)->socket_context;
}


struct fj_socket const * fj_device_context_get_socket(
    void const * device_context
)
{
    return fj_socket_context_get_socket(
        fj_device_context_get_socket_context(device_context)
    );
}
