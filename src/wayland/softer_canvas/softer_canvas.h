#include <fejix/interface/softer_canvas.h>

#include <src/wayland/client/client.h>

#include <fejix/core/vec.h>


struct fj_wayland_softer_canvas_global_data {
    struct fj_softer_canvas_callbacks callbacks;
    struct wl_shm */*?*/ shm;

    /** Contains fj_softer_pixel_format_id_t */
    struct fj_vec pixel_formats;
};
