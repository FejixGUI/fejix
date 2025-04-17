#ifndef FEJIX_WINDOW_H_INCLUDED
#define FEJIX_WINDOW_H_INCLUDED


#include <fejix/modules/app.h>


FJ_OPAQUE_STRUCT_WITH_USERDATA(fj_window_manager)
FJ_OPAQUE_STRUCT_WITH_USERDATA(fj_window)


FJ_METHOD_NONNULL(
    fj_window_create_manager,
    fj_err,
    struct fj_app_manager *app_manager,
    struct fj_window_manager **out_manager)

FJ_METHOD(fj_window_destroy_manager, fj_err, struct fj_window_manager *manager)

/** The window must be synced in order to be operational. */
FJ_METHOD(
    fj_window_create, fj_err, struct fj_window_manager *manager, struct fj_window **out_window)

FJ_METHOD(fj_window_destroy, fj_err, struct fj_window_manager *manager, struct fj_window *window)

/** Synchronizes the window, all of its contents, and most of its dependent objects. */
FJ_METHOD(fj_window_sync, fj_err, struct fj_window_manager *manager, struct fj_window *window)

FJ_METHOD_LIST_BEGIN(fj_window)
FJ_METHOD_LIST_ITEM(fj_window_create_manager)
FJ_METHOD_LIST_ITEM(fj_window_destroy_manager)
FJ_METHOD_LIST_ITEM(fj_window_create)
FJ_METHOD_LIST_ITEM(fj_window_destroy)
FJ_METHOD_LIST_ITEM(fj_window_sync)
FJ_METHOD_LIST_END()


#endif
