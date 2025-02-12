#ifndef FEJIX_VIEW_ACTIVITY_H_
#define FEJIX_VIEW_ACTIVITY_H_


#include <fejix/interface/view.h>


typedef uint32_t fj_view_activity_flags_t;

// TODO: rename this to focus hints and add to one of the future input interfaces.
enum fj_view_activity_flags {
    FJ_SCENE_ACTIVITY_NONDISMISSABLE = 1 << 0,

    /** Inhibits the screensaver. */
    FJ_SCENE_ACTIVITY_BUSY = 1 << 1,

    /**
        Disables the screen with a proximity sensor (prevents the ear to react with the
        touchscreen).

        TODO: On Android the wake lock can be released immediately, so that the screen is turned
        back on without waiting for the proximity sensor. This is a really weird behavior, but just
        in case... we can implement it as a separate hint that slightly changes the behavior.
        Or... we can implement that behavior using FJ_SCENE_ACTIVITY_BUSY.
    */
    FJ_SCENE_ACTIVITY_PHONE_CALL = 1 << 2,
};


struct fj_scene_activity_flags_manager;

struct fj_scene_activity_flags_functions {
    fj_err_t (*create_manager)(
        struct fj_scene_manager *owner_manager,
        struct fj_scene_activity_flags_manager **out_manager);

    fj_err_t (*destroy_manager)(struct fj_scene_activity_flags_manager *manager);

    fj_view_activity_flags_t (*get_supported_flags)(
        struct fj_scene_activity_flags_manager *manager, struct fj_scene *scene);

    fj_err_t (*set_flags)(
        struct fj_scene_activity_flags_manager *manager,
        struct fj_scene *scene,
        fj_view_activity_flags_t flags);
};


#endif
