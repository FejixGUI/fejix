#ifndef FEJIX_SCENE_H_
#define FEJIX_SCENE_H_


#include <fejix/interface/app.h>
#include <fejix/interface/image_consumer.h>

#include <fejix/core/primitives.h>


struct fj_scene_manager;
struct fj_scene;

struct fj_scene_create_info {
    void *userdata;
};

struct fj_scene_manager_callbacks {
    fj_err_t (*on_content_geometry_update)(
        struct fj_app *app, struct fj_scene *scene, struct fj_content_geometry const *geometry);

    fj_err_t (*on_dismissed)(struct fj_app *app, struct fj_scene *scene);
};

struct fj_scene_manager_create_info {
    struct fj_scene_manager_callbacks const *callbacks;
};

struct fj_scene_functions {
    fj_err_t (*create_manager)(
        struct fj_app *owner_app,
        struct fj_scene_manager **out_manager,
        struct fj_scene_manager_create_info const *info);

    fj_err_t (*destroy_manager)(struct fj_scene_manager *manager);

    fj_err_t (*create_scene)(
        struct fj_scene_manager *manager,
        struct fj_scene **out_scene,
        struct fj_scene_create_info const *info);

    /** Destroying a scene inside one of its callbacks is undefined behavior. */
    fj_err_t (*destroy_scene)(struct fj_scene_manager *manager, struct fj_scene *scene);

    void (*get_image_consumer)(
        struct fj_scene_manager *manager,
        struct fj_scene *scene,
        struct fj_image_consumer **out_consumer);

    fj_err_t (*dismiss_scene)(struct fj_scene_manager *manager, struct fj_scene *scene);

    /** :param scenes: Must contain at least one scene. */
    fj_err_t (*update_scenes)(
        struct fj_scene_manager *manager, struct fj_scene *const *scenes, uint32_t scene_count);
};


#endif
