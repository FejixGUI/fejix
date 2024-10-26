#ifndef FEJIX_INTERFACE_SOFTER_H_
#define FEJIX_INTERFACE_SOFTER_H_


#include <fejix/interface/canvas.h>
#include <fejix/interface/client.h>


typedef uint32_t fj_softer_pixel_format_t;

enum fj_softer_pixel_format {
    /** (Red:8,Green:8,Blue:8), endianness applicable */
    FJ_SOFTER_PIXEL_FORMAT_RGB24,

    /** (Unused:8,Red:8,Green:8,Blue:8), endianness applicable */
    FJ_SOFTER_PIXEL_FORMAT_XRGB32,
    /** (Alpha:8,Red:8,Green:8,Blue:8), alpha premultiplied, endianness applicable */
    FJ_SOFTER_PIXEL_FORMAT_ARGB32,
    /** (Red:8,Green:8,Blue:8,Unused:8), endianness applicable */
    FJ_SOFTER_PIXEL_FORMAT_RGBX32,
    /** (Red:8,Green:8,Blue:8,Alpha:8), alpha premultiplied, endianness applicable */
    FJ_SOFTER_PIXEL_FORMAT_RGBA32,

    FJ_SOFTER_PIXEL_FORMAT_COLOR_MAX,

    FJ_SOFTER_PIXEL_FORMAT_COLOR_MASK = 0xFFFF,

    FJ_SOFTER_PIXEL_FORMAT_LITTLE_ENDIAN = (1 << 16),
};


struct fj_softer_manager;
struct fj_softer_canvas_context;


struct fj_softer_manager_desc {
    /** NULL if empty. */
    fj_softer_pixel_format_t const *pixel_formats;
    uint32_t pixel_format_count;
};

struct fj_softer_canvas_info {
    union fj_tag tag;
    struct fj_size2d size;
    fj_softer_pixel_format_t pixel_format;
};

struct fj_softer_canvas_context_desc {
    uint8_t *pixel_data;
    uint32_t stride;
    uint32_t age;
};


struct fj_softer_callbacks {
    fj_err_t (*update_canvas)(
        struct fj_client *client,
        struct fj_canvas *canvas,
        struct fj_softer_canvas_context *context,
        struct fj_softer_canvas_context_desc const *context_desc
    );
};

struct fj_softer_funcs {
    fj_err_t (*create_manager)(
        struct fj_softer_manager **manager,
        struct fj_softer_manager_desc *manager_desc,
        struct fj_client *client,
        struct fj_softer_callbacks const *callbacks
    );

    fj_err_t (*destroy_manager)(struct fj_softer_manager *manager);

    fj_err_t (*create_canvas)(
        struct fj_softer_manager *manager,
        struct fj_canvas **canvas,
        struct fj_softer_canvas_info const *canvas_info
    );

    fj_err_t (*destroy_canvas)(struct fj_softer_manager *manager, struct fj_canvas *canvas);

    fj_err_t (*update_canvas)(
        struct fj_softer_manager *manager,
        struct fj_softer_canvas_context *context,
        struct fj_softer_canvas_info const *canvas_info
    );

    fj_err_t (*present_canvas)(
        struct fj_softer_manager *manager,
        struct fj_softer_canvas_context *context
    );
};


FJ_EXTERN_C_BEGIN


/** Returns 0 for unknown formats. */
size_t fj_softer_get_pixel_size(fj_softer_pixel_format_t pixel_format);


FJ_EXTERN_C_END

#endif
