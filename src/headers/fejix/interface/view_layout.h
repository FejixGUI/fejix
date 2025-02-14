#ifndef FEJIX_SCENE_LAYOUT_INCLUDED
#define FEJIX_SCENE_LAYOUT_INCLUDED


#include <fejix/interface/scene.h>


struct fj_scene_layout;


struct fj_scene_layout_callbacks {
    fj_err_t (*on_update_layout)(
        struct fj_app *app, struct fj_scene *scene, struct fj_scene_layout *layout);
};

struct fj_scene_layout_functions {
    fj_err_t (*set_needs_update_layout)(struct fj_scene_manager *manager, struct fj_scene *scene);
};


#endif
