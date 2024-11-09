#ifndef FEJIX_INTERFACE_CANVAS_H_
#define FEJIX_INTERFACE_CANVAS_H_


#include <fejix/core/base.h>


struct fj_canvas FJ_PUBLICLY({ union fj_tag tag; });

/** All canvas functions are internal to implementations. */
struct fj_canvas_funcs;

#endif