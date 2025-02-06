#include <fejix/interface/softer.h>

#include <src/wayland/client/client.h>
#include <src/wayland/output/output.h>

#include <src/unixshm/unixshm.h>

#include <fejix/core/vec.h>


struct fj_wayland_softer_canvas_global_data {
    struct fj_softer_canvas_callbacks callbacks;

    struct wl_shm * shm;

    /** Contains fj_softer_pixel_format_id_t */
    struct fj_vec pixel_formats;
};


struct fj_wayland_softer_canvas_buffer {
    struct fj_unixshm shm;
    struct wl_shm_pool * pool;
    struct wl_buffer * buffer;
    fj_bool32_t available;
    uint32_t age; // TODO

};


struct fj_wayland_softer_canvas {
    struct fj_wayland_canvas_base base;

    struct fj_wayland_softer_canvas_buffer buffers[2];
    struct fj_softer_canvas_info info;
    uint32_t active_buffer;
    uint32_t available_buffer_count;
};
