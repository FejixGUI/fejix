#ifndef FEJIX_BUS_H_
#define FEJIX_BUS_H_


#include <fejix/base.h>


typedef uint32_t fj_bus_id_t;

/** The variants are sorted alphabetically. */
enum fj_bus_id {
    /** Android Native Development Kit */
    FJ_BUS_ANDK = 1,
    /** Apple Cocoa */
    FJ_BUS_COCOA,
    /** No-operation testing bus that contains no sockets and is basically a
        dummy. This may be useful for some cases when you need to build the
        library without any real buses. */
    FJ_BUS_NOOP,
    /** Apple UI Kit */
    FJ_BUS_UIKIT,
    /** Wayland protocol */
    FJ_BUS_WAYLAND,
    /** Windows API */
    FJ_BUS_WINAPI,
    /** X11 protocol */
    FJ_BUS_X11,
};

typedef uint32_t fj_socket_id_t;

enum fj_socket_id {
    FJ_SOCKET_WINDOW = 1,
    FJ_SOCKET_GEOMETRY,
    FJ_SOCKET_BUS_WAKEUP,
};

typedef uint32_t fj_message_id_t;

enum fj_generic_message_id {
    FJ_MSG_SOCKET_DEVICE_ADDED = 1,
    FJ_MSG_SOCKET_DEVICE_REMOVED,

    /** Sets whether the user needs to receive regular DEVICE_ADDED/REMOVED
        messages, in other words, if the user wants to monitor the devices.
        If the user does NOT want to monitor the socket devices, the socket
        may skip regular updating of the device list, which may prevent some
        unnecessary overhead.

        The opened devices will always receive their DEVICE_REMOVED
        message, regardless of this setting.

        The initial sequence of DEVICE_ADDED messages will always be issued
        by every socket when committing the first time after the socket was
        opened.

        The socket can ignore this setting and always monitor its devices. */
    FJ_MSG_SOCKET_SET_DEVICE_MONITOR_REQUIRED,
};

typedef uint32_t fj_serve_type_t;

enum fj_serve_type {
    /** Represents most kinds of main program entrypoints. */
    FJ_SERVE_TYPE_MAIN = 1,
};

typedef uint32_t fj_bus_schedule_command_t;

enum fj_bus_schedule_command {
    FJ_SCHEDULE_WAIT_FOREVER = 0,
    FJ_SCHEDULE_WAIT_INTERVAL,
    FJ_SCHEDULE_QUIT,
};

struct fj_bus_schedule {
    fj_bus_schedule_command_t command;
    uint32_t interval_ms;
};

/** Must be the first field of every bus context. */
struct fj_bus_context_base {
    struct fj_bus const * bus;

    /** The array is sorted by socket IDs. */
    struct fj_socket const * const * FJ_ARRAY sockets;
    uint32_t socket_count;

    struct fj_bus_schedule schedule;
    void * FJ_NULLABLE user_data;
};

/** Must be the first field of every socket context. */
struct fj_socket_context_base {
    struct fj_socket const * socket;
    void * bus_context;
};

/** Must be the first field of every device context. */
struct fj_device_context_base {
    void * socket_context;
    void * device_handle;
    void * FJ_NULLABLE dependent_device_context;
};

struct fj_message {
    void * socket_context;
    void * FJ_NULLABLE device_context;
    void * message_data;
    fj_message_id_t message;
};

typedef fj_err_t (fj_bus_listener_t)(
    void * bus_context,
    uint32_t message_count,
    struct fj_message const * FJ_ARRAY messages
);


struct fj_bus {
    fj_bus_id_t id;
    fj_version_t version;

    fj_err_t (* open)(
        void * FJ_NULLABLE FJ_OUT * bus_context,
        fj_bus_listener_t * bus_listener
    );

    void (* close)(
        void * bus_context
    );

    fj_err_t (* serve)(
        void * bus_context,
        fj_serve_type_t serve_type,
        void * FJ_NULLABLE serve_data
    );

    /** Commits all the buffered changes to the shell.
        This must be called inside the `serve` method. */
    fj_err_t (* commit)(
        void * bus_context
    );
};


struct fj_socket {
    fj_socket_id_t id;
    void const * FJ_NULLABLE interface;
    void const * FJ_NULLABLE private;

    fj_err_t (* open)(
        void * bus_context,
        void * FJ_NULLABLE FJ_OUT * socket_context
    );

    void (* close)(
        void * socket_context
    );

    /** Device info should contain at least the device handle. */
    fj_err_t (* open_device)(
        void * socket_context,
        void * device_info,
        void * FJ_NULLABLE FJ_OUT * device_context
    );

    void (* close_device)(
        void * device_context
    );

    /** Buffers the message. */
    fj_err_t (* send)(
        struct fj_message const * message
    );
};


/** The returned array is sorted by bus IDs.
    Bus count is always greater than 0. */
void fj_get_buses(
    uint32_t FJ_OUT * bus_count,
    struct fj_bus const * const * FJ_ARRAY FJ_OUT * buses
);


#endif
