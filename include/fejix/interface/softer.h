#ifndef FEJIX_SOFTER_H_
#define FEJIX_SOFTER_H_


#include <fejix/interface/client.h>
#include <fejix/interface/canvas.h>


FJ_DEFINE_OPAQUE_TYPE(fj_softer_canvas_t)


typedef uint32_t fj_softer_pixel_format_t;

enum fj_softer_pixel_format {
    /** R:8,G:8,B:8, endianness applicable */
    FJ_SOFTER_PIXEL_FORMAT_RGB24,
    /** R:8,G:8,B:8,X:32, endianness applicable */
    FJ_SOFTER_PIXEL_FORMAT_RGBX32,
    /** R:8,G:8,B:8,A:32, endianness applicable */
    FJ_SOFTER_PIXEL_FORMAT_RGBA32,
    /** X:32,R:8,G:8,B:8, endianness applicable */
    FJ_SOFTER_PIXEL_FORMAT_XRGB32,
    /** A:32,R:8,G:8,B:8, endianness applicable */
    FJ_SOFTER_PIXEL_FORMAT_ARGB32,

    FJ_SOFTER_PIXEL_FORMAT_BIG_ENDIAN = 0x00000,
    FJ_SOFTER_PIXEL_FORMAT_LITTLE_ENDIAN = 0x10000,
    FJ_SOFTER_PIXEL_FORMAT_COLOR = 0x0FFFF,
    FJ_SOFTER_PIXEL_FORMAT_ENDIANNESS = 0x10000,
};


struct fj_softer_desc {
    fj_softer_pixel_format_t const */*[]*/ pixel_formats;
    uint32_t pixel_format_count;
};

struct fj_softer_canvas_info {
    struct fj_size2d size;
    fj_softer_pixel_format_t pixel_format;
};

struct fj_softer_canvas_desc {
    uint8_t */*[]*/ pixel_data;
    uint32_t stride;
};


struct fj_softer_callbacks {
    fj_err_t (* init)(void */*?*/ data, struct fj_softer_desc const */*?*/ desc);

    fj_err_t (* update)(
        void */*?*/ data,
        fj_softer_canvas_t * canvas,
        struct fj_softer_canvas_desc const * desc
    );
};

struct fj_softer_iface {
    fj_err_t (* init)(
        fj_client_t * client,
        struct fj_softer_callbacks const * callbacks
    );

    fj_err_t (* create)(
        fj_client_t * client,
        fj_softer_canvas_t */*? out*/ * canvas,
        struct fj_softer_canvas_info const * info
    );

    fj_err_t (* destroy)(fj_client_t * client, fj_softer_canvas_t * canvas);

    fj_err_t (* update)(
        fj_client_t * client,
        fj_softer_canvas_t * canvas,
        struct fj_softer_canvas_info const * info
    );

    fj_err_t (* present)(fj_client_t * client, fj_softer_canvas_t * canvas);
};


#endif
