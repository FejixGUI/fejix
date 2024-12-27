#ifndef FEJIX_INTERFACE_SOFTER_H_
#define FEJIX_INTERFACE_SOFTER_H_


#include <fejix/interface/client.h>
#include <fejix/interface/window.h>


typedef uint32_t fj_softer_manager_flags_t;

enum fj_softer_manager_flags {
    FJ_SOFTER_MANAGER_IMMEDIATE_PRESENT = 1,
};


typedef uint32_t fj_softer_pixel_format_t;

enum fj_softer_pixel_format {
    FJ_SOFTER_PIXEL_FORMAT_RGB24,
    FJ_SOFTER_PIXEL_FORMAT_BGR24,

    FJ_SOFTER_PIXEL_FORMAT_SIZE24_MAX = FJ_SOFTER_PIXEL_FORMAT_BGR24,

    FJ_SOFTER_PIXEL_FORMAT_XRGB32,
    FJ_SOFTER_PIXEL_FORMAT_BGRX32,
    FJ_SOFTER_PIXEL_FORMAT_ARGB32,
    FJ_SOFTER_PIXEL_FORMAT_BGRA32,
    FJ_SOFTER_PIXEL_FORMAT_RGBX32,
    FJ_SOFTER_PIXEL_FORMAT_XBGR32,
    FJ_SOFTER_PIXEL_FORMAT_RGBA32,
    FJ_SOFTER_PIXEL_FORMAT_ABGR32,

    FJ_SOFTER_PIXEL_FORMAT_SIZE32_MAX = FJ_SOFTER_PIXEL_FORMAT_ABGR32,

    FJ_SOFTER_PIXEL_FORMAT_MAX = FJ_SOFTER_PIXEL_FORMAT_SIZE32_MAX,
};


struct fj_softer_manager;
struct fj_softer_canvas_builder;
struct fj_softer_canvas;
struct fj_softer_canvas_buffer;


struct fj_softer_manager_info {
    /** NULL if empty. */
    fj_softer_pixel_format_t const *pixel_formats;
    uint32_t pixel_format_count;

    fj_softer_manager_flags_t flags;
};

struct fj_softer_canvas_buffer_info {
    uint8_t *pixel_data;
    uint32_t stride;
    uint32_t age;
    fj_bool8_t available;
};


struct fj_softer_interface {
    fj_err_t (*create_manager)(
        struct fj_softer_manager **manager,
        struct fj_client *client,
        struct fj_softer_manager_info *manager_info
    );

    fj_err_t (*destroy_manager)(struct fj_softer_manager *manager);

    fj_err_t (*create_canvas_builder)(
        struct fj_softer_manager *manager,
        struct fj_softer_canvas_builder **canvas_builder,
        struct fj_window_builder *window_builder,
        fj_softer_pixel_format_t pixel_format
    );

    fj_err_t (*destroy_canvas_builder)(
        struct fj_softer_manager *manager,
        struct fj_softer_canvas_builder *canvas_builder
    );

    fj_err_t (*create_canvas)(
        struct fj_softer_manager *manager,
        struct fj_softer_canvas_builder *canvas_builder,
        struct fj_softer_canvas **canvas,
        struct fj_window *window
    );

    fj_err_t (*destroy_canvas)(
        struct fj_softer_manager *manager,
        struct fj_softer_canvas *canvas,
        struct fj_window *window
    );

    fj_err_t (*create_canvas_buffer)(
        struct fj_softer_manager *manager,
        struct fj_softer_canvas *canvas,
        struct fj_softer_canvas_buffer **canvas_buffer,
        struct fj_size2d const *size
    );

    fj_err_t (*destroy_canvas_buffer)(
        struct fj_softer_manager *manager,
        struct fj_softer_canvas_buffer *canvas_buffer,
        struct fj_softer_canvas *canvas
    );

    fj_err_t (*set_canvas_buffer_size)(
        struct fj_softer_manager *manager,
        struct fj_softer_canvas *canvas,
        struct fj_softer_canvas_buffer *canvas_buffer,
        struct fj_size2d const *size
    );

    fj_err_t (*get_canvas_buffer_info)(
        struct fj_softer_manager *manager,
        struct fj_softer_canvas *canvas,
        struct fj_softer_canvas_buffer *canvas_buffer,
        struct fj_softer_canvas_buffer_info *canvas_buffer_info
    );

    fj_err_t (*present_canvas)(
        struct fj_softer_manager *manager,
        struct fj_softer_canvas *canvas,
        struct fj_softer_canvas_buffer *canvas_buffer
    );
};


/** Returns 0 for unknown formats. */
FJ_API
size_t fj_softer_get_pixel_size(fj_softer_pixel_format_t pixel_format);


#endif
