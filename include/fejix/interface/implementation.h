/**
This is the only mandatory interface that an implementation must provide.
*/

#ifndef FEJIX_IMPLEMENTATION_H_
#define FEJIX_IMPLEMENTATION_H_


#include <fejix/core/base.h>


typedef uint32_t fj_implementation_id_t;

enum fj_implementation_id {
    /** X11 protocol */
    FJ_IMPLEMENTATION_X11,
    /** Wayland protocol */
    FJ_IMPLEMENTATION_WAYLAND,
    /** Android Native Development Kit */
    FJ_IMPLEMENTATION_ANDK,
    /** Apple Cocoa */
    FJ_IMPLEMENTATION_COCOA,
    /** Apple UIKit */
    FJ_IMPLEMENTATION_UIKIT,
    /** Windows API */
    FJ_IMPLEMENTATION_WINAPI,
    /** Universal Windows Platform */
    FJ_IMPLEMENTATION_UWP,

    /** */
    FJ_IMPLEMENTATION_PREDEFINED_COUNT,

    /** Minimal ID for user-defined implementations. */
    FJ_IMPLEMENTATION_USER = 0x1000,
};


void fj_implementation_get_id(fj_implementation_id_t *out_id);

void fj_implementation_get_name(char const **out_name);

void fj_implementation_get_version(fj_version_t *out_version);


#endif
