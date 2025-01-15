#ifndef FEJIX_IMAGE_CONTAINER_H_
#define FEJIX_IMAGE_CONTAINER_H_


#include <fejix/core/base.h>


/**
    An image (a.k.a. pixel map or surface buffer) is a two-dimensional array of pixels.

    An image container (a.k.a. surface) is an ordered set of zero or more images containing
    image metadata that is used for a single or multiple continuous drawing operations.

    A typical example of an image container is a pair of a back image, which is used for
    drawing the next frame, and a front image, which is used for screen scan-out, i.e.
    presenting a drawn frame. In this case the images are constantly swapped after the back image
    gets fully drawn.

    Image container is a part of every object that displays images on screen.
    Image containers are retrieved from such objects and populated with displayable images by means
    of graphical API interfaces like RAM, OpenGL, Vulkan etc.

    Because images can be created in a variety of different ways, they are generally considered
    opaque objects whose data is only known to the image-creating interfaces.
*/
struct fj_image_container;


#endif
