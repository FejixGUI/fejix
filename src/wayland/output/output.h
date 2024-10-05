#ifndef FJ_WAYLAND_LAYER_H_
#define FJ_WAYLAND_LAYER_H_

#include <fejix/interface/implementation.h>
#include <fejix/interface/output.h>

#include <src/wayland/client/client.h>

struct fj_wayland_output_global_data {
  struct fj_output_callbacks callbacks;
  struct wl_compositor *compositor;
};

struct fj_wayland_canvas_base {
  struct fj_wayland_output * /*?*/ output;
};

struct fj_wayland_output {
  struct fj_wayland_canvas_base *canvas;
  struct wl_surface *surface;
  struct fj_output_info info;
};

#endif
