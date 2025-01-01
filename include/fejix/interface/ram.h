#ifndef FEJIX_INTERFACE_RAM_H_
#define FEJIX_INTERFACE_RAM_H_


#include <fejix/interface/window.h>


typedef uint32_t fj_ram_image_get_flags_t;

enum fj_ram_image_get_flags {
    FJ_RAM_IMAGE_GET_IMMEDIATE = 0,
    FJ_RAM_IMAGE_GET_WAIT_AVAILABLE = 1,
};

typedef uint32_t fj_ram_pixel_format_t;

enum fj_ram_pixel_format {
    FJ_RAM_PIXEL_FORMAT_RGB24,
    FJ_RAM_PIXEL_FORMAT_BGR24,
    FJ_RAM_PIXEL_FORMAT_SIZE24_MAX = FJ_RAM_PIXEL_FORMAT_BGR24,
    FJ_RAM_PIXEL_FORMAT_XRGB32,
    FJ_RAM_PIXEL_FORMAT_BGRX32,
    FJ_RAM_PIXEL_FORMAT_ARGB32,
    FJ_RAM_PIXEL_FORMAT_BGRA32,
    FJ_RAM_PIXEL_FORMAT_RGBX32,
    FJ_RAM_PIXEL_FORMAT_XBGR32,
    FJ_RAM_PIXEL_FORMAT_RGBA32,
    FJ_RAM_PIXEL_FORMAT_ABGR32,
    FJ_RAM_PIXEL_FORMAT_SIZE32_MAX = FJ_RAM_PIXEL_FORMAT_ABGR32,
    FJ_RAM_PIXEL_FORMAT_MAX = FJ_RAM_PIXEL_FORMAT_SIZE32_MAX,
};


struct fj_ram_manager;

struct fj_ram_image_set_create_info {
    struct fj_image_usage_context *image_usage_context;
    fj_ram_pixel_format_t pixel_format;
    struct fj_size initial_size;
};

struct fj_ram_image_info {
    uint8_t *pixels;
    uint32_t stride;
    uint32_t age;
};


struct fj_ram_funcs {
    fj_err_t (*get)(struct fj_app *app, struct fj_ram_manager **out_manager);

    fj_err_t (*release)(struct fj_ram_manager *manager);

    fj_err_t (*create_image_set)(
        struct fj_ram_manager *manager,
        struct fj_image_set **out_image_set,
        struct fj_ram_image_set_create_info const *info
    );

    fj_err_t (*destroy_image_set)(struct fj_ram_manager *manager, struct fj_image_set *image_set);

    fj_err_t (*get_current_image_info)(
        struct fj_ram_manager *manager,
        struct fj_image_set *image_set,
        struct fj_ram_image_info *out_info,
        fj_ram_image_get_flags_t flags
    );

    fj_err_t (*present_current_image)(
        struct fj_ram_manager *manager,
        struct fj_image_set *image_set
    );
};


/** Returns 0 for unknown formats. */
FJ_EXPORT
size_t fj_ram_get_pixel_size(fj_ram_pixel_format_t pixel_format);


#endif
