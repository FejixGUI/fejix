#ifndef FEJIX_WINAPI_IMAGE_CONTAINER_INCLUDED
#define FEJIX_WINAPI_IMAGE_CONTAINER_INCLUDED


#include <fejix/core/geometry.h>

#include <windows.h>


enum {
    FJ_WINAPI_IMAGE_CONTAINER_WINDOW,
    FJ_WINAPI_IMAGE_CONTAINER_CUSTOM_OPENGL,
};


struct fj_image_container {
    enum type;
    struct fj_size size;

    /** Used by the interfaces that create images. */
    void *images;

    HWND window;
    HDC device_context;
    HBITMAP bitmap;
};


#endif
