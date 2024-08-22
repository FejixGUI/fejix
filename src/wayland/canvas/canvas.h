#ifndef FEJIX_WAYLAND_CANVAS_H_
#define FEJIX_WAYLAND_CANVAS_H_


#include <fejix/interface/canvas.h>
#include <fejix/interface/implementation.h>

#include <src/wayland/client/client.h>


struct fj_wayland_canvas {
    fj_interface_id_t interface_id;
};


#endif
