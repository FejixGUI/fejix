#ifndef FEJIX_WINAPI_IMAGE_CONTAINER_H_
#define FEJIX_WINAPI_IMAGE_CONTAINER_H_


#include <fejix/core/geometry.h>

#include <windows.h>


enum fj_winapi_image_container_type {
    FJ_WINAPI_IMAGE_CONTAINER_WINDOW,
    FJ_WINAPI_IMAGE_CONTAINER_CUSTOM_OPENGL,
};


struct fj_image_container {
    enum fj_winapi_image_container_type type;
    struct fj_size size;

    /** Used by the interfaces that create images. */
    void *images;

    HWND window;
    HDC device_context;
    HBITMAP bitmap;
};


#endif
