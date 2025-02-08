#ifndef FEJIX_IMAGE_H_
#define FEJIX_IMAGE_H_


#include <fejix/core/primitives.h>


struct fj_image_create_event;
struct fj_image_destroy_event;
struct fj_image_present_event;

struct fj_image_size_event {
    struct fj_size size;
};


#endif
