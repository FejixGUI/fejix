#ifndef FEJIX_IMPLEMENTATION_H_
#define FEJIX_IMPLEMENTATION_H_


#include <fejix/base.h>


/** The variants are sorted alphabetically except for NOOP. */
enum fj_implementation_id {
    /** No-operation testing bus that contains no sockets and is basically a
        dummy. This may be useful for some cases when you need to build the
        library without any real buses. */
    FJ_IMPLEMENTATION_NOOP,
    /** Android Native Development Kit */
    FJ_IMPLEMENTATION_ANDK,
    /** Apple Cocoa */
    FJ_IMPLEMENTATION_COCOA,
    /** Apple UI Kit */
    FJ_IMPLEMENTATION_UIKIT,
    /** Wayland protocol */
    FJ_IMPLEMENTATION_WAYLAND,
    /** Windows API */
    FJ_IMPLEMENTATION_WINAPI,
    /** X11 protocol */
    FJ_IMPLEMENTATION_X11,
};

enum fj_interface_id {
    FJ_INSTANCE,
    FJ_SERVER,
    FJ_COMPOSITOR,
    FJ_DISPLAY,
};

struct fj_message {
    fj_enum32_t sender_interface_id;
    fj_enum32_t message_id;
    void * sender_object;
    void *fjOPTION message_data;
};

struct fj_implementation {
    fj_enum32_t implementation_id;

    void *fjOPTION static_data;

    void const *fjOPTION (* get_interface)(
        struct fj_implementation const * implementation,
        fj_enum32_t interface_id
    );
};


/** The returned array is sorted by bus IDs.
    Bus count is always greater than 0. */
void fj_get_builtin_implementations(
    uint32_t fjOUT * implementation_count,
    struct fj_implementation const * const *fjARRAY fjOUT * implementations
);


#endif
