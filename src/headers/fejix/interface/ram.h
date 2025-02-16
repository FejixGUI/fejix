#ifndef FEJIX_RAM_INCLUDED
#define FEJIX_RAM_INCLUDED


#include <fejix/interface/view.h>


/** */
typedef uint32_t fj_ram_pixel_format_t;

/** */
enum {
    /** */
    FJ_RAM_PIXEL_FORMAT_RGB24,
    /** */
    FJ_RAM_PIXEL_FORMAT_BGR24,
    /** */
    FJ_RAM_PIXEL_FORMAT_XRGB32,
    /** */
    FJ_RAM_PIXEL_FORMAT_BGRX32,
    /** */
    FJ_RAM_PIXEL_FORMAT_ARGB32,
    /** */
    FJ_RAM_PIXEL_FORMAT_BGRA32,
    /** */
    FJ_RAM_PIXEL_FORMAT_RGBX32,
    /** */
    FJ_RAM_PIXEL_FORMAT_XBGR32,
    /** */
    FJ_RAM_PIXEL_FORMAT_RGBA32,
    /** */
    FJ_RAM_PIXEL_FORMAT_ABGR32,

    /** */
    FJ_RAM_PIXEL_FORMAT_COUNT,
};


/** */
struct fj_ram_manager;
/** */
struct fj_ram_images;

/** */
struct fj_ram_image_info {
    uint8_t *pixels;
    uint32_t stride;
    uint32_t age;
};


fj_version_t fj_ram_get_interface_version(void);

/** */
fj_err_t fj_ram_create_manager(struct fj_app *owner_app, struct fj_ram_manager **out_manager);

/** */
fj_err_t fj_ram_destroy_manager(struct fj_ram_manager *manager);

/** */
void fj_ram_get_supported_formats(
    struct fj_ram_manager *manager,
    fj_ram_pixel_format_t **out_formats,
    uint32_t *out_count,
    struct fj_image_binding *image_binding);

/** */
fj_err_t fj_ram_create_images(
    struct fj_ram_manager *manager,
    struct fj_ram_images **out_images,
    fj_ram_pixel_format_t format,
    struct fj_image_binding *image_binding);

fj_err_t fj_ram_destroy_images(struct fj_ram_manager *manager, struct fj_ram_images *images);

/**
    May reallocate images.
    Any available images in use should be retrieved and redrawn again after this call.

    The intended way is to resize images as soon as the consumer is resized and start drawing
    only after that.
*/
fj_err_t fj_ram_resize_images(struct fj_ram_manager *manager, struct fj_ram_images *images);

/** Blocks until the next image can be acquired. */
fj_err_t fj_ram_wait_image_available(struct fj_ram_manager *manager, struct fj_ram_images *images);

/**
    May fail if previous swap operations are not finished. To synchronize, use
    ``wait_image_available()``.
*/
fj_err_t fj_ram_get_available_image_info(
    struct fj_ram_manager *manager,
    struct fj_ram_images *images,
    struct fj_ram_image_info *out_image_info);

/**
    This is an asynchronous operation, getting an available image may fail when called right
    after this function.
    To synchronize, use ``wait_image_available()``.

    Has no effect on image sets that have one image (for which this does not make sense).
*/
fj_err_t fj_ram_present_image(struct fj_ram_manager *manager, struct fj_ram_images *images);


#endif
