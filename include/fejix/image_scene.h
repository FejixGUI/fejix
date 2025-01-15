#ifndef FEJIX_IMAGE_SCENE_H_
#define FEJIX_IMAGE_SCENE_H_


#include <fejix/app.h>
#include <fejix/image_container.h>

#include <fejix/core/geometry.h>


/** The length of a metric inch in metres. */
#define FJ_INCH_LENGTH (0.0025)


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

struct fj_image_scene_geometry_info {
    struct fj_size size;

    /**
        Measured in PPM (pixels-per-metre).

        To calculate PPI (pixels-per-inch), multiply by ``FJ_INCH_LENGTH``.
        The legacy "normal" PPI that was very wide-spread is 96 pixels per inch.
        Concepts like text/interface scaling factor is derived from the ratio to that value.
        That is, if the current PPI is 120, the content of the appriate size is considered to be
        scaled by 120 / 96 = 125% compared to the "unscaled" (density-unaware) content rendered at
        96 PPI.
    */
    double density;

    fj_orientation_type_t orientation;
};

struct fj_image_scene_manager_callbacks {
    fj_err_t (*on_geometry_update)(
        struct fj_app *app,
        struct fj_image_scene *image_scene,
        struct fj_image_scene_geometry_info const *update_info);

    fj_err_t (*on_dismissed)(struct fj_app *app, struct fj_image_scene *image_scene);
};

struct fj_image_scene_manager_create_info {
    struct fj_image_scene_manager_callbacks const *callbacks;
};

struct fj_image_scene_funcs {
    void const *(*get_interface_funcs)(fj_image_scene_interface_id_t id);

    fj_err_t (*create_manager)(
        struct fj_app *owner_app,
        struct fj_image_scene_manager **out_manager,
        struct fj_image_scene_manager_create_info const *info);

    fj_err_t (*destroy_manager)(struct fj_image_scene_manager *manager);

    fj_err_t (*create_image_scene)(
        struct fj_image_scene_manager *manager,
        struct fj_image_scene **out_image_scene,
        struct fj_image_scene_create_info const *info);

    /** Destroying a scene inside one of its callbacks is undefined behavior. */
    fj_err_t (*destroy_image_scene)(
        struct fj_image_scene_manager *manager, struct fj_image_scene *image_scene);

    void (*get_image_container)(
        struct fj_image_scene_manager *manager,
        struct fj_image_scene *image_scene,
        struct fj_image_container **out_image_container);

    /** :param image_scenes: Must contain at least one scene. */
    fj_err_t (*update)(
        struct fj_image_scene_manager *manager,
        struct fj_image_scene *const *image_scenes,
        uint32_t image_scene_count);
};


#endif
