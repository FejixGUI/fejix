#ifndef FEJIX_IMAGE_SET_H_
#define FEJIX_IMAGE_SET_H_


#include <fejix/core/base.h>


/**
    An image (a.k.a. pixel map or surface buffer) is a two-dimensional array of
    pixels.

    An image set (a.k.a. surface) is a set of one or more images that is
    suitable for a single or multiple continuous drawing operations.

    Because images can be created in a variety of different ways, they are generally considered
    opaque objects, whose data is only known to the image-creating interfaces.

    The requirement to specify an image set object while creating a consumer object is often
    a trick to keep things more general and cross-platform.
*/
struct fj_image_set;

/**
    A specification of how an image is to be accessed, which influences how the the image should
    be created.

    Often serves as an image creation hint from image-consuming interfaces to image-producing
    interfaces.
*/
struct fj_image_access_context;


#endif
