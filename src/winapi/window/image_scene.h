#ifndef FEJIX_WINAPI_IMAGE_SCENE_INCLUDED
#define FEJIX_WINAPI_IMAGE_SCENE_INCLUDED


#include <src/winapi/image_container/image_container.h>

#include <fejix/image_scene.h>


struct fj_image_scene_manager {
    struct fj_app *app;
    struct fj_image_scene_manager_callbacks const *callbacks;
};

struct fj_image_scene {
    union fj_tag tag;

    struct fj_image_scene_manager *manager;

    struct fj_image_container image_container;
    HWND window;
};


#endif
