#ifndef FEJIX_INTERFACE_IMAGE_H_
#define FEJIX_INTERFACE_IMAGE_H_


/**
    An image is a two-dimensional array of pixels.

    Depending on context, may also be referred to as
    bit map, pixel map, pixel buffer, image buffer, window buffer, window surface, backing store,
    frame, or frame buffer.

    Because images can be created in a variaety of different ways, they are generally considered
    opaque objects, whose data is only known to the image-creating interfaces.
*/
struct fj_image;

/**
    A set of one or more images that is suitable for a single or multiple continuous drawing
    operations.

    Depending on context, may be referred to as canvas.
*/
struct fj_image_set;

/** This is known to both image-creating and image-consuming interfaces. */
struct fj_image_sharing_info;


#endif
