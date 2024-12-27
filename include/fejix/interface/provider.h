#ifndef FEJIX_INTERFACE_PROVIDER_H_
#define FEJIX_INTERFACE_PROVIDER_H_


#include <fejix/core/base.h>


typedef uint32_t fj_provider_id_t;

enum fj_provider_id {
    /** Android Native Development Kit */
    FJ_PROVIDER_ANDK,
    /** Apple Cocoa */
    FJ_PROVIDER_COCOA,
    /** Wayland protocol */
    FJ_PROVIDER_WAYLAND,
    /** Windows API */
    FJ_PROVIDER_WINAPI,
    /** X11 protocol */
    FJ_PROVIDER_X11,

    FJ_PROVIDER_MAX = FJ_PROVIDER_X11,
};


typedef uint32_t fj_interface_id_t;

enum fj_interface_id {
    FJ_INTERFACE_CLIENT,
    FJ_INTERFACE_WINDOW,
    FJ_INTERFACE_SOFTER,
    FJ_INTERFACE_OPENGL,

    FJ_INTERFACE_MAX = FJ_INTERFACE_OPENGL,
};


enum fj_provider_request_id {
    FJ_PROVIDER_GET_ID,
    FJ_PROVIDER_GET_VERSION,
    FJ_PROVIDER_GET_DEFAULT_RESPONDER,
};


FJ_DECLARE_ABSTRACT_OBJECT(fj_provider)


#endif
