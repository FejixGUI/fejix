/**
    \file
*/

#ifndef FEJIX_WINDOW_H_INCLUDED
#define FEJIX_WINDOW_H_INCLUDED


#include <fejix/modules/io_thread.h>


FJ_OBJECT_TYPE(fj_window_manager)
FJ_OBJECT_TYPE(fj_window)


FJ_METHOD_WITH_FALLBACK(
    fj_window_create_manager,
    enum fj_error,
    FJ_ERROR_UNIMPLEMENTED,
    struct fj_io_thread *io_thread,
    struct fj_window_manager **out_manager)

FJ_METHOD(fj_window_destroy_manager, enum fj_error, struct fj_window_manager *manager)

/** The window must be synced in order to be operational. */
FJ_METHOD(
    fj_window_create,
    enum fj_error,
    struct fj_window_manager *manager,
    struct fj_window **out_window)

FJ_METHOD(
    fj_window_destroy, enum fj_error, struct fj_window_manager *manager, struct fj_window *window)

/** Synchronizes the window, all of its contents, and most of its dependent objects. */
FJ_METHOD(
    fj_window_sync, enum fj_error, struct fj_window_manager *manager, struct fj_window *window)

FJ_METHOD_LIST_BEGIN(fj_window)
FJ_METHOD_LIST_ITEM(fj_window_create_manager)
FJ_METHOD_LIST_ITEM(fj_window_destroy_manager)
FJ_METHOD_LIST_ITEM(fj_window_create)
FJ_METHOD_LIST_ITEM(fj_window_destroy)
FJ_METHOD_LIST_ITEM(fj_window_sync)
FJ_METHOD_LIST_END()


#endif
