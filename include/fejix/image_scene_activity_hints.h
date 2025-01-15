#ifndef FEJIX_IMAGE_SCENE_ACTIVITY_HINTS_H_
#define FEJIX_IMAGE_SCENE_ACTIVITY_HINTS_H_


#include <fejix/image_scene.h>


typedef uint32_t fj_image_scene_activity_hint_t;

enum fj_image_scene_activity_hint {
    /** Inhibits the screensaver. */
    FJ_IMAGE_SCENE_ACTIVITY_BUSY,

    /** TODO: regular updates for videos. */
    FJ_IMAGE_SCENE_ACTIVITY_PLAYBACK,

    /** TODO: urgent updates for games. */
    FJ_IMAGE_SCENE_ACTIVITY_REALTIME,

    /**
        Disables the screen with a proximity sensor (prevents the ear to react with the
        touchscreen).

        TODO: On Android the wake lock can be released immediately, so that the screen is turned
        back on without waiting for the proximity sensor. This is a really weird behavior, but just
        in case... we can implement it as a separate hint that slightly changes the behavior.
    */
    FJ_IMAGE_SCENE_ACTIVITY_PHONE_CALL,
};


struct fj_image_scene_activity_hints_manager;

struct fj_image_scene_activity_hints_funcs {
    fj_err_t (*create_manager)(
        struct fj_image_scene_manager *owner_manager,
        struct fj_image_scene_activity_hints_manager **out_manager
    );

    fj_err_t (*destroy_manager)(struct fj_image_scene_activity_hints_manager *manager);

    fj_bool8_t (*get_hint_supported)(
        struct fj_image_scene_activity_hints_manager *manager,
        struct fj_image_scene *image_scene,
        fj_image_scene_activity_hint_t hint
    );

    fj_err_t (*set_hint)(
        struct fj_image_scene_activity_hints_manager *manager,
        struct fj_image_scene *image_scene,
        fj_image_scene_activity_hint_t hint,
        fj_bool8_t value
    );
};


#endif
