#ifndef FEJIX_APP_WIN_IMAGE_SET_H_
#define FEJIX_APP_WIN_IMAGE_SET_H_


#include <windows.h>


enum fj_win_image_access_type {
    FJ_WIN_IMAGE_ACCESS_SCENE,
};


struct fj_image_set {
    /** Used by the interfaces that create images. */
    void *images;

    HWND window;
    HDC device_context;
};


#endif
