#ifndef FEJIX_WINAPI_IMAGE_SET_H_
#define FEJIX_WINAPI_IMAGE_SET_H_


#include <fejix/core/geometry.h>

#include <windows.h>


enum fj_winapi_image_set_type {
    FJ_WINAPI_IMAGE_SET_WINDOW,
    FJ_WINAPI_IMAGE_SET_BITMAP,
    FJ_WINAPI_IMAGE_SET_OPENGL_PBUFFER,
};


struct fj_image_set {
    enum fj_winapi_image_set_type type;
    struct fj_size size;

    /** Used by the interfaces that create images. */
    void *images;

    HWND window;
    HDC device_context;
    HBITMAP bitmap;
};


// TODO Functions to automatically create windows or other objects for image sets.
// (window procedure, etc.)


#endif
