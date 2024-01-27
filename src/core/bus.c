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


void fj_bus_set_callback_data(
    struct fj_bus const * bus,
    void * bus_context,
    void * callback_data
)
{
    bus->set_callback_data(bus_context, callback_data);
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
    struct fj_socket * socket,
    void * bus_context,
    void * socket_context
)
{
    socket->close(bus_context, socket_context);
}


fj_err_t fj_socket_open_device(
    struct fj_socket * socket,
    void * bus_context,
    void * socket_context,
    void * device_handle,
    void * FJ_NULLABLE device_open_info,
    void * FJ_NULLABLE FJ_OUT * device_context
)
{
    return socket->open_device(
        bus_context,
        socket_context,
        device_handle,
        device_open_info,
        device_context
    );
}


void fj_socket_close_device(
    struct fj_socket * socket,
    void * bus_context,
    void * socket_context,
    void * device_context
)
{
    socket->close_device(bus_context, socket_context, device_context);
}


fj_bool_t fj_socket_supports(
    struct fj_socket * socket,
    void * bus_context,
    void * socket_context,
    void * FJ_NULLABLE device_context,
    fj_message_id_t message_id
)
{
    return socket->supports(
        bus_context,
        socket_context,
        device_context,
        message_id
    );
}
