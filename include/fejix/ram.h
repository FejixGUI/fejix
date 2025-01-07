#ifndef FEJIX_RAM_H_
#define FEJIX_RAM_H_


#include <fejix/image_scene.h>


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

struct fj_ram_image_set_capabilities {
    fj_ram_pixel_format_t *supported_formats;
    uint32_t supported_format_count;
};

struct fj_ram_image_set_create_info {
    struct fj_image_compatibility_context *image_compatibility_context;
    fj_ram_pixel_format_t pixel_format;
    struct fj_size initial_size;
};

struct fj_ram_image {
    uint8_t *pixels;
    uint32_t stride;
    uint32_t age;
};


struct fj_ram_funcs {
    fj_err_t (*create_manager)(struct fj_app *owner_app, struct fj_ram_manager **out_manager);

    fj_err_t (*destroy_manager)(struct fj_ram_manager *manager);

    void (*get_image_set_capacilities)(
        struct fj_ram_manager *manager,
        struct fj_image_compatibility_context *compatibility_context,
        struct fj_ram_image_set_capabilities *out_capabilities
    );

    fj_err_t (*create_image_set)(
        struct fj_ram_manager *manager,
        struct fj_image_set **out_image_set,
        struct fj_ram_image_set_create_info const *info
    );

    fj_err_t (*destroy_image_set)(struct fj_ram_manager *manager, struct fj_image_set *image_set);

    /** May fail if previous swap operations are not finished. To synchronize, use ``sync()``. */
    fj_err_t (*acquire_next_image)(
        struct fj_ram_manager *manager,
        struct fj_image_set *image_set,
        struct fj_ram_image *out_image
    );

    /** Blocks until the next image can be acquired. */
    fj_err_t (*sync)(struct fj_ram_manager *manager, struct fj_image_set *image_set);

    /**
        This is an asynchronous operation, acquiring the next available image may fail when called
        after this function.
        To synchronize, use ``sync()``.

        Has no effect on image sets that have one image (for which this does not make sense).
    */
    fj_err_t (*swap_images)(struct fj_ram_manager *manager, struct fj_image_set *image_set);
};


/** Returns 0 for unknown formats. */
FJ_PUBLIC
size_t fj_ram_get_pixel_size(fj_ram_pixel_format_t pixel_format);


#endif
