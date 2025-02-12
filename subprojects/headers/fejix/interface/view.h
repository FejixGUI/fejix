#ifndef FEJIX_VIEW_H_
#define FEJIX_VIEW_H_


#include <fejix/interface/app.h>
#include <fejix/interface/image.h>

#include <fejix/core/primitives.h>


struct fj_view_manager;
struct fj_view;


fj_version_t fj_view_get_interface_version(void);

fj_err_t fj_view_create_manager(struct fj_app *owner_app, struct fj_view_manager **out_manager);

fj_err_t fj_view_destroy_manager(struct fj_view_manager *manager);

fj_err_t fj_view_create(struct fj_view_manager *manager, struct fj_view **out_view, void *userdata);

/** Destroying a view inside one of its callbacks is undefined behavior. */
fj_err_t fj_view_destroy(struct fj_view_manager *manager, struct fj_view *view);

/** Not binding images inside the callback is platform-specific behavior. */
typedef fj_err_t (*fj_view_on_bind_images_fn_t)(
    void *userdata, struct fj_image_bind_context *context);

void fj_view_on_bind_images(struct fj_view_manager *manager, fj_view_on_bind_images_fn_t callback);

typedef fj_err_t (*fj_view_on_dismiss_fn_t)(void *userdata);

void fj_view_on_dismiss(struct fj_view_manager *manager, fj_view_on_dismiss_fn_t callback);

typedef fj_err_t (*fj_view_on_update_geometry_fn_t)(
    void *userdata, struct fj_geometry const *geometry);

void fj_view_on_update_geometry(
    struct fj_view_manager *manager, fj_view_on_update_geometry_fn_t callback);

/** :param views: Must contain at least one view. */
fj_err_t fj_view_update_batch(
    struct fj_view_manager *manager, struct fj_view *const *views, uint32_t view_count);


#endif
