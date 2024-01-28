#ifndef FEJIX_BUS_H_
#define FEJIX_BUS_H_


#include <fejix/base.h>


typedef uint32_t fj_bus_id_t;

/** The variants are sorted alphabetically. */
enum fj_bus_id {
    /** Android Native Development Kit */
    FJ_BUS_ANDK,
    /** Apple Cocoa */
    FJ_BUS_COCOA,
    /** Apple UI Kit */
    FJ_BUS_UIKIT,
    /** Wayland protocol */
    FJ_BUS_WAYLAND,
    /** Windows API */
    FJ_BUS_WINAPI,
    /** X11 protocol */
    FJ_BUS_X11,
};

typedef uint32_t fj_serve_type_t;

enum fj_serve_type {
    /** Represents most kinds of main program entrypoints. */
    FJ_SERVE_TYPE_MAIN,
};

typedef uint32_t fj_socket_id_t;

enum fj_socket_id {
    FJ_SOCKET_WINDOW,
    FJ_SOCKET_WINDOW_GEOMETRY,
};

typedef uint32_t fj_message_id_t;

enum fj_message_id {
    FJ_MSG_BUS_OPENED,
    FJ_MSG_BUS_CLOSING,
    FJ_MSG_BUS_CLOSE,
    FJ_MSG_SOCKET_DEVICE_ADDED,
    FJ_MSG_SOCKET_DEVICE_REMOVING,
    FJ_MSG_SOCKET_DEVICES_UPDATE,
};

/** Must be the first field of every bus context.
    This enables the usage of `fj_bus_context_get_xxx()`. */
struct fj_bus_context_base {
    struct fj_bus const * bus;
    void * FJ_NULLABLE user_data;
};

/** Must be the first field of every socket context.
    This enables the usage of `fj_socket_context_get_xxx()`. */
struct fj_socket_context_base {
    struct fj_socket const * socket;
    void * bus_context;
};

/** Must be the first field of every device context.
    This enables the usage of `fj_device_context_get_xxx()`. */
struct fj_device_context_base {
    void * socket_context;
    void * device_handle;
    void * FJ_NULLABLE parent_context;
};

struct fj_message {
    fj_message_id_t message;
    void * device_context;
    void * message_data;
};

typedef fj_err_t (fj_bus_listener_t)(
    void * bus_context,
    uint32_t message_count,
    struct fj_message const * FJ_ARRAY messages
);


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

    fj_err_t (* open_device)(
        void * socket_context,
        void * device_handle,
        void * FJ_NULLABLE device_open_info,
        void * FJ_NULLABLE FJ_OUT * device_context
    );

    void (* close_device)(
        void * device_context
    );

    fj_bool_t (* supports)(
        void * device_context,
        fj_message_id_t message_id
    );
};

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

    /** The returned array is sorted by socket IDs. */
    void (* get_sockets)(
        void * bus_context,
        uint32_t FJ_OUT * socket_count,
        struct fj_socket const * const * FJ_ARRAY FJ_OUT * sockets
    );

    fj_err_t (* serve)(
        void * bus_context,
        fj_serve_type_t serve_type,
        void * FJ_NULLABLE serve_data
    );

    /** Sends the messages in a single batch.
        The order of the messages may not be preseved.

        This must be called inside the `serve` method. */
    fj_err_t (* send)(
        void * bus_context,
        uint32_t message_count,
        struct fj_message const * FJ_ARRAY messages
    );
};


/** The returned array is sorted by bus IDs.
    Bus count is always greater than 0. */
void fj_get_buses(
    uint32_t FJ_OUT * bus_count,
    struct fj_bus const * const * FJ_ARRAY FJ_OUT * buses
);

fj_err_t fj_bus_open(
    struct fj_bus const * bus,
    void * FJ_NULLABLE FJ_OUT * bus_context,
    fj_bus_listener_t * bus_listener
);

void fj_bus_close(
    struct fj_bus const * bus,
    void * bus_context
);

void fj_bus_get_sockets(
    struct fj_bus const * bus,
    void * bus_context,
    uint32_t FJ_OUT * socket_count,
    struct fj_socket const * const * FJ_ARRAY FJ_OUT * sockets
);

fj_err_t fj_bus_serve(
    struct fj_bus const * bus,
    void * bus_context,
    fj_serve_type_t serve_type,
    void * FJ_NULLABLE serve_data
);

fj_err_t fj_bus_send(
    struct fj_bus const * bus,
    void * bus_context,
    uint32_t message_count,
    struct fj_message const * FJ_ARRAY messages
);

fj_err_t fj_socket_open(
    struct fj_socket * socket,
    void * bus_context,
    void * FJ_NULLABLE FJ_OUT * socket_context
);

void fj_socket_close(
    void * socket_context
);

fj_err_t fj_socket_open_device(
    void * socket_context,
    void * device_handle,
    void * FJ_NULLABLE device_open_info,
    void * FJ_NULLABLE FJ_OUT * device_context
);

void fj_socket_close_device(
    void * device_context
);

fj_bool_t fj_socket_supports(
    void * device_context,
    fj_message_id_t message_id
);

struct fj_bus const * fj_bus_context_get_bus(
    void const * bus_context
);

void * FJ_NULLABLE fj_bus_context_get_user_data(
    void const * bus_context
);

void fj_bus_context_set_user_data(
    void * bus_context,
    void * FJ_NULLABLE user_data
);

struct fj_socket const * fj_socket_context_get_socket(
    void const * socket_context
);

void * fj_socket_context_get_bus_context(
    void const * socket_context
);

struct fj_bus const * fj_socket_context_get_bus(
    void const * socket_context
);

void * FJ_NULLABLE fj_device_context_get_parent(
    void const * device_context
);

void * fj_device_context_get_handle(
    void const * device_context
);

void * fj_device_context_get_socket_context(
    void const * device_context
);

struct fj_socket const * fj_device_context_get_socket(
    void const * device_context
);


#endif
