#ifndef FJ_OPENGL_H_INCLUDED
#define FJ_OPENGL_H_INCLUDED


#include <fejix/io.h>


enum { FJ_OPENGL_PROTOCOL = 17 };


typedef uint32_t fj_image_attribute_t;

enum {
    FJ_IMAGE_COLOR_FORMAT,
    FJ_IMAGE_DEPTH_BITS,
    FJ_IMAGE_STENCIL_BITS,
};


typedef struct fj_opengl_swapchain fj_opengl_swapchain_t;


typedef struct fj_opengl {
    fj_err_t (*create_swapchain)(fj_io_connection_t *conn, fj_opengl_swapchain_t *out_swapchain);
} fj_opengl_t;


#endif
