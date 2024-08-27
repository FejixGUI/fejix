#ifndef FJ_WAYLAND_LAYER_H_
#define FJ_WAYLAND_LAYER_H_


#include <fejix/interface/layer.h>

#include <src/wayland/client/client.h>


struct fj_wayland_layer_global_data {
    struct fj_layer_callbacks callbacks;
    struct wl_compositor */*?*/ compositor;
};


struct fj_wayland_layer {
    fj_canvas_t * canvas;
    struct wl_surface * surface;
    struct fj_size2d size;
    fj_layer_flags_t flags;
};


#endif
