#ifndef FEJIX_IMAGE_CONSUMER_H_
#define FEJIX_IMAGE_CONSUMER_H_


#include <fejix/core/base.h>


/**
    An image (a.k.a. a pixel map or a surface buffer) is a two-dimensional array of pixels.

    An image consumer (a.k.a. a surface) is an object that receives images, for example,
    to display them.
*/
struct fj_image_consumer;


struct fj_image_consumer_funcs {
    void (*get_desired_size)(struct fj_image_consumer *image_consumer, struct fj_size *out_size);
};


#endif
