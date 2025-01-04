#ifndef FEJIX_INTERFACE_IMAGE_SCENE_H_
#define FEJIX_INTERFACE_IMAGE_SCENE_H_


#include <fejix/interface/app.h>
#include <fejix/interface/image_set.h>

#include <fejix/core/geometry.h>


typedef uint32_t fj_image_scene_manager_extension_id_t;

enum fj_image_scene_manager_extension_id {
    FJ_IMAGE_SCENE_MANAGER_EXTENSION_SYNC_MANAGER,
};


struct fj_image_scene_manager;
FJ_DEFINE_TAGGED_STRUCT(fj_image_scene)

struct fj_image_scene_create_info {
    struct fj_image_access_context *image_access_context;
    struct fj_image_set *image_set;
};

struct fj_image_scene_update_info {
    struct fj_density density;
    struct fj_size size;
    fj_orientation_type_t orientation;
};

struct fj_image_scene_manager_callbacks {
    fj_err_t (*on_update_image_scene)(
        struct fj_app *app,
        struct fj_image_scene *image_scene,
        struct fj_image_scene_update_info const *info
    );
};

struct fj_image_scene_manager_create_info {
    struct fj_image_scene_manager_callbacks const *callbacks;
};

struct fj_image_scene_manager_funcs {
    void const *(*get_extension)(fj_image_scene_manager_extension_id_t id);

    fj_err_t (*create_manager)(
        struct fj_image_scene_manager **out_manager,
        struct fj_app *owner_app,
        struct fj_image_scene_manager_create_info const *info
    );

    fj_err_t (*destroy_manager)(struct fj_image_scene_manager *manager);

    fj_err_t (*get_image_access_context)(
        struct fj_image_scene_manager *manager,
        struct fj_image_access_context **out_image_access_context
    );

    fj_err_t (*release_image_access_context)(
        struct fj_image_scene_manager *manager,
        struct fj_image_access_context *image_access_context
    );

    fj_err_t (*create_image_scene)(
        struct fj_image_scene_manager *manager,
        struct fj_image_scene **out_image_scene,
        struct fj_image_scene_create_info const *info
    );

    fj_err_t (*destroy_image_scene)(
        struct fj_image_scene_manager *manager,
        struct fj_image_scene *image_scene
    );

    fj_err_t (*set_image_scene_needs_update)(
        struct fj_image_scene_manager *manager,
        struct fj_image_scene *image_scene
    );

    fj_err_t (*update_image_scenes)(
        struct fj_image_scene_manager *manager,
        struct fj_image_scene *const *image_scenes,
        uint32_t image_scene_count
    );
};


#endif
