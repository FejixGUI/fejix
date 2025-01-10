#ifndef FEJIX_IMAGE_SCENE_H_
#define FEJIX_IMAGE_SCENE_H_


#include <fejix/app.h>
#include <fejix/image_set.h>

#include <fejix/core/geometry.h>


typedef uint32_t fj_image_scene_interface_id_t;

enum fj_image_scene_interface_id {
    FJ_IMAGE_SCENE_INTERFACE_SYNC,
    FJ_IMAGE_SCENE_INTERFACE_LAYOUT,
};


struct fj_image_scene_manager;
struct fj_image_scene;

FJ_PUBLICLY_TAGGED(fj_image_scene)

struct fj_image_scene_create_info {
    union fj_tag tag;
};

struct fj_image_scene_content_update_info {
    struct fj_density density;
    struct fj_size size;
    fj_orientation_type_t orientation;
};

struct fj_image_scene_manager_callbacks {
    fj_err_t (*on_content_update)(
        struct fj_app *app,
        struct fj_image_scene *image_scene,
        struct fj_image_scene_content_update_info const *info
    );
};

struct fj_image_scene_manager_create_info {
    struct fj_image_scene_manager_callbacks const *callbacks;
};

struct fj_image_scene_funcs {
    void const *(*get_interface_funcs)(fj_image_scene_interface_id_t id);

    fj_err_t (*create_manager)(
        struct fj_app *owner_app,
        struct fj_image_scene_manager **out_manager,
        struct fj_image_scene_manager_create_info const *info
    );

    fj_err_t (*destroy_manager)(struct fj_image_scene_manager *manager);

    fj_err_t (*create_image_scene)(
        struct fj_image_scene_manager *manager,
        struct fj_image_scene **out_image_scene,
        struct fj_image_scene_create_info const *info
    );

    fj_err_t (*destroy_image_scene)(
        struct fj_image_scene_manager *manager,
        struct fj_image_scene *image_scene
    );

    void (*get_image_set)(
        struct fj_image_scene_manager *manager,
        struct fj_image_scene *image_scene,
        struct fj_image_set **out_image_set
    );

    fj_err_t (*set_needs_content_update)(
        struct fj_image_scene_manager *manager,
        struct fj_image_scene *image_scene
    );

    fj_err_t (*update)(
        struct fj_image_scene_manager *manager,
        struct fj_image_scene *const *image_scenes,
        uint32_t image_scene_count
    );
};


#endif
