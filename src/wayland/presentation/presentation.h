#ifndef FJ_WAYLAND_LAYER_H_
#define FJ_WAYLAND_LAYER_H_


#include <fejix/interface/implementation.h>
#include <fejix/interface/presentation.h>

#include <src/wayland/client/client.h>


struct fj_wayland_presentation_global_data {
    struct fj_presentation_callbacks callbacks;
    struct wl_compositor * compositor;
};


struct fj_wayland_canvas_base {
    fj_interface_id_t interface_id;
};


struct fj_wayland_presentation {
    struct fj_wayland_canvas_base * canvas;
    struct wl_surface * surface;
    struct fj_size2d size;
    fj_presentation_flags_t flags;
};


#endif
