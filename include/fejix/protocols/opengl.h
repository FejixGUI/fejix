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


struct fj_opengl_swapchain;


struct fj_opengl {
    fj_err_t (*create_swapchain)(
        struct fj_io_connection *conn,
        struct fj_opengl_swapchain **out_swapchain
    );
};


#endif
