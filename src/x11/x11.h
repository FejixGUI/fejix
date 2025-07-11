#ifndef FEJIX_X11_H_
#define FEJIX_X11_H_


#include <fejix/base.h>

#include <X11/Xlib.h>
#include <xcb/xcb.h>


char const *fj_x11_error_into_string(uint8_t error_code);
char const *fj_x11_xcb_error_into_string(xcb_generic_error_t *error);


#endif
