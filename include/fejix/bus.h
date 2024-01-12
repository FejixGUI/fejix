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
    /** X11 protocol for the Xorg server */
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

enum fj_bus_message_id {
    FJ_MESSAGE_BUS_SETUP,
    FJ_MESSAGE_BUS_SHUTDOWN,
};

struct fj_message {
    fj_socket_id_t socket_id;
    fj_message_id_t message;
    void const * message_transport;
    void * socket_context;
    void * device_context;
    void * message_data;
};

typedef fj_err_t (fj_bus_listener_t)(
    void * callback_data,
    uint32_t message_count,
    struct fj_message const * FJ_ARRAY messages
);


struct fj_socket {
    fj_socket_id_t id;
    void const * methods;
    void const * message_transport;
    //TODO
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

    void (* set_callback_data)(
        void * bus_context,
        void * callback_data
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

void fj_bus_set_callback_data(
    struct fj_bus const * bus,
    void * bus_context,
    void * callback_data
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


#endif
