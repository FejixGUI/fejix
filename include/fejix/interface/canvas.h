#ifndef FEJIX_INTERFACE_CANVAS_H_
#define FEJIX_INTERFACE_CANVAS_H_


#include <fejix/core/base.h>


struct fj_canvas FJ_PUBLICLY({ union fj_tag tag; });


struct fj_canvas_iface {
    uint8_t dummy;
};

#endif
