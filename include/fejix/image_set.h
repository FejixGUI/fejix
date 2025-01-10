#ifndef FEJIX_IMAGE_SET_H_
#define FEJIX_IMAGE_SET_H_


#include <fejix/core/base.h>


/**
    An image (a.k.a. pixel map or surface buffer) is a two-dimensional array of
    pixels.

    An image set (a.k.a. surface) is a set of zero or more images that is
    used for a single or multiple continuous drawing operations.

    Image set is a property of every object that displays images on screen.
    Image sets are retrieved from such objects and populated with displayable images by means
    of graphical API interfaces like RAM, OpenGL, Vulkan etc.

    Because images can be created in a variety of different ways, they are generally considered
    opaque objects whose data is only known to the image-creating interfaces.
*/
struct fj_image_set;


#endif
