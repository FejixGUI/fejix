#ifndef FEJIX_SCENE_LAYOUT_SIZE_INCLUDED
#define FEJIX_SCENE_LAYOUT_SIZE_INCLUDED


#include <fejix/interface/scene_layout.h>


typedef uint32_t fj_scene_layout_size_hint_flags_t;

enum fj_scene_layout_size_hint_flags {
    FJ_SCENE_LAYOUT_SIZE_HINT_PREFERRED_SIZE = 1,
    FJ_SCENE_LAYOUT_SIZE_HINT_MIN_SIZE = 1 << 1,
    FJ_SCENE_LAYOUT_SIZE_HINT_MAX_SIZE = 1 << 2,
    FJ_SCENE_LAYOUT_SIZE_HINT_SIZE_INCREMENTS = 1 << 3,
};


struct fj_scene_layout_size_hints {
    struct fj_size preferred_size;
    struct fj_size min_size;
    struct fj_size max_size;
    struct fj_size size_increments;
};


struct fj_scene_layout_size_functions {
    fj_err_t (*set_size_hints)(
        struct fj_app *app,
        struct fj_scene_layout *layout,
        struct fj_scene_layout_size_hints const *hints,
        fj_scene_layout_size_hint_flags_t hint_flags);
};


#endif
