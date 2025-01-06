#ifndef FEJIX_WINAPI_IMAGE_SCENE_H_
#define FEJIX_WINAPI_IMAGE_SCENE_H_


#include <fejix/image_scene.h>

#include <windows.h>


struct fj_image_scene_manager {
    struct fj_app *app;
    struct fj_image_scene_manager_callbacks const *callbacks;
};

struct fj_image_scene {
    union fj_tag tag;

    HWND window;
};


#endif
