#ifndef FEJIX_SCENE_H_
#define FEJIX_SCENE_H_


#include <fejix/app.h>
#include <fejix/image_consumer.h>


struct fj_scene_manager;
struct fj_scene;

struct fj_scene_create_info {
    void *userdata;
};

struct fj_scene_geometry_info {
    struct fj_size size;
    fj_dots_per_metre_t pixel_density;
    fj_orientation_type_t orientation;
};

struct fj_scene_manager_callbacks {
    fj_err_t (*on_geometry_update)(
        struct fj_app *app,
        struct fj_scene *scene,
        struct fj_scene_geometry_info const *update_info);

    fj_err_t (*on_dismissed)(struct fj_app *app, struct fj_scene *scene);
};

struct fj_scene_manager_create_info {
    struct fj_scene_manager_callbacks const *callbacks;
};

struct fj_scene_funcs {
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

    struct fj_image_consumer (*get_image_consumer)(
        struct fj_scene_manager *manager, struct fj_scene *scene);

    /** :param scenes: Must contain at least one scene. */
    fj_err_t (*update_scenes)(
        struct fj_scene_manager *manager, struct fj_scene *const *scenes, uint32_t scene_count);
};


#endif
