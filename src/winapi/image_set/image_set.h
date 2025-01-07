#ifndef FEJIX_WINAPI_IMAGE_SET_H_
#define FEJIX_WINAPI_IMAGE_SET_H_


#include <windows.h>


enum fj_winapi_image_access_type {
    FJ_WINAPI_IMAGE_ACCESS_IMAGE_SCENE,
};


struct fj_image_set {
    /** Used by the interfaces that create images. */
    void *images;

    HWND window;
    HDC device_context;
};

struct fj_image_compatibility_context {
    enum fj_winapi_image_access_type access_type;
};

// TODO Functions to automatically create windows or other objects for image sets.
// (window procedure, etc.)


#endif
