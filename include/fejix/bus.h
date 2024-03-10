#ifndef FEJIX_BUS_H_
#define FEJIX_BUS_H_


#include <fejix/base.h>


#define FJ_FIRST_MESSAGE_ID(SOCKET_ID) ((SOCKET_ID)<<16)


/** The variants are sorted alphabetically except for NOOP. */
enum fj_bus_id {
    /** No-operation testing bus that contains no sockets and is basically a
        dummy. This may be useful for some cases when you need to build the
        library without any real buses. */
    FJ_BUS_NOOP,
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

enum fj_socket_id {
    FJ_SOCKET_GENERIC,
    FJ_SOCKET_SCHEDULER,
    FJ_SOCKET_WINDOW,
    FJ_SOCKET_SURFACE,
    FJ_SOCKET_POINTER_INPUT,
    FJ_SOCKET_KEYBOARD_INPUT,
    FJ_SOCKET_TOUCHPAD_INPUT,
    FJ_SOCKET_BLANK,
    FJ_SOCKET_RASTER,
    FJ_SOCKET_OPENGL,
    FJ_SOCKET_VULKAN,
};

enum fj_generic_message_id {
    FJ_GENERIC_SOCKET_OPEN = FJ_FIRST_MESSAGE_ID(FJ_SOCKET_GENERIC),
    FJ_GENERIC_SOCKET_CLOSE,
    FJ_GENERIC_DEVICE_ADD,
    FJ_GENERIC_DEVICE_REMOVE,
    FJ_GENERIC_DEVICE_OPEN,
    FJ_GENERIC_DEVICE_CLOSE,
};

enum fj_serve_type {
    /** Represents most kinds of main program entrypoints. */
    FJ_SERVE_TYPE_MAIN,
};


struct fj_socket_context_base FJ_INHERITABLE {
    void * bus_context;
    struct fj_socket const * socket;
};


struct fj_device_context_base FJ_INHERITABLE {
    void * socket_context;
    void * device;
};


struct fj_message {
    void * socket_context;
    void * FJ_NULLABLE device_context;
    fj_id_t message;
    void * FJ_NULLABLE message_data;
};


typedef fj_err_t (fj_bus_listener_t)(
    void * bus_context,
    struct fj_message const * message
);


struct fj_bus_context_base FJ_INHERITABLE {
    struct fj_bus const * bus;

    /** The array is sorted by socket IDs. */
    struct fj_socket const * const * FJ_NULLABLE FJ_ARRAY sockets;
    uint32_t socket_count;

    fj_bus_listener_t * listener;

    void * FJ_NULLABLE user_data;
};


struct fj_bus {
    fj_id_t id;
    fj_version_t version;

    fj_err_t (* open)(
        void * FJ_NULLABLE FJ_OUT * bus_context
    );

    void (* close)(
        void * bus_context
    );

    fj_err_t (* serve)(
        void * bus_context,
        fj_id_t serve_type,
        void * FJ_NULLABLE serve_data
    );
};


struct fj_socket {
    fj_id_t id;
    void const * FJ_NULLABLE interface;
    void const * FJ_NULLABLE private;

    /** This is a buffered operation. */
    fj_err_t (* open)(
        void * bus_context,
        void * FJ_NULLABLE FJ_OUT * socket_context
    );

    void (* close)(
        void * socket_context
    );

    fj_err_t (* open_device)(
        void * socket_context,
        void * device,
        void * FJ_NULLABLE FJ_OUT * device_context
    );

    void (* close_device)(
        void * device_context
    );

    /** Buffers the message. Unsupported messages may be ignored. */
    fj_err_t (* send)(
        struct fj_message const * message
    );

    fj_bool_t (* supports)(
        void * device_context,
        fj_id_t capability
    );
};


/** The returned array is sorted by bus IDs.
    Bus count is always greater than 0. */
void fj_get_buses(
    uint32_t FJ_OUT * bus_count,
    struct fj_bus const * const * FJ_ARRAY FJ_OUT * buses
);


#endif
