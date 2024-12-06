#ifndef FEJIX_INTERFACE_SOFTER_H_
#define FEJIX_INTERFACE_SOFTER_H_


#include <fejix/interface/client.h>
#include <fejix/interface/window.h>


typedef uint32_t fj_softer_pixel_format_t;

enum fj_softer_pixel_format {
    /** (Red:8,Green:8,Blue:8), endianness applicable */
    FJ_SOFTER_PIXEL_FORMAT_RGB24,

    /** (Zeroes:8,Red:8,Green:8,Blue:8), endianness applicable */
    FJ_SOFTER_PIXEL_FORMAT_ZRGB32,
    /** (Alpha:8,Red:8,Green:8,Blue:8), alpha premultiplied, endianness applicable */
    FJ_SOFTER_PIXEL_FORMAT_ARGB32,
    /** (Red:8,Green:8,Blue:8,Zeroes:8), endianness applicable */
    FJ_SOFTER_PIXEL_FORMAT_RGBZ32,
    /** (Red:8,Green:8,Blue:8,Alpha:8), alpha premultiplied, endianness applicable */
    FJ_SOFTER_PIXEL_FORMAT_RGBA32,

    FJ_SOFTER_PIXEL_FORMAT_COLOR_MAX,

    FJ_SOFTER_PIXEL_FORMAT_COLOR_MASK = 0x0FFF,

    FJ_SOFTER_PIXEL_FORMAT_LITTLE_ENDIAN = 0x1000,
};


struct fj_softer_manager;
struct fj_softer_canvas_builder;
struct fj_softer_canvas;


struct fj_softer_manager_info {
    /** NULL if empty. */
    fj_softer_pixel_format_t const *pixel_formats;
    uint32_t pixel_format_count;
};

struct fj_softer_canvas_create_info {
    union fj_tag tag;
    struct fj_size2d size;
    fj_softer_pixel_format_t pixel_format;
};

struct fj_softer_canvas_info {
    uint8_t *pixel_data;
    uint32_t stride;
    uint32_t age;
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
        struct fj_softer_canvas_create_info const *create_info
    );

    fj_err_t (*destroy_builder)(
        struct fj_softer_manager *manager,
        struct fj_softer_canvas_builder *canvas_builder
    );

    fj_err_t (*create_canvas)(
        struct fj_softer_manager *manager,
        struct fj_softer_canvas **canvas,
        struct fj_softer_canvas_builder *canvas_builder,
        struct fj_window *window
    );

    fj_err_t (*destroy_canvas)(
        struct fj_softer_manager *manager,
        struct fj_softer_canvas *canvas,
        struct fj_window *window
    );

    fj_err_t (*get_canvas_info)(
        struct fj_softer_manager *manager,
        struct fj_softer_canvas *canvas,
        struct fj_softer_canvas_info *canvas_info
    );

    fj_err_t (*resize_canvas)(
        struct fj_softer_manager *manager,
        struct fj_softer_canvas *canvas,
        struct fj_size2d const *size
    );

    fj_err_t (*present_canvas)(struct fj_softer_manager *manager, struct fj_softer_canvas *canvas);
};


FJ_EXTERN_C_BEGIN

/** Returns 0 for unknown formats. */
size_t fj_softer_get_pixel_size(fj_softer_pixel_format_t pixel_format);

FJ_EXTERN_C_END

#endif
