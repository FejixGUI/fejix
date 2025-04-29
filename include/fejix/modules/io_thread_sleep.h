#ifndef FEJIX_IO_THREAD_SLEEP_H_INCLUDED
#define FEJIX_IO_THREAD_SLEEP_H_INCLUDED


#include <fejix/modules/io_thread.h>


FJ_OBJECT_TYPE(fj_io_thread_sleep_manager)


FJ_METHOD_WITH_FALLBACK(
    fj_io_thread_sleep_create_manager,
    enum fj_error,
    FJ_ERROR_UNIMPLEMENTED,
    struct fj_io_thread *io_thread,
    struct fj_io_thread_sleep_manager **out_manager)

FJ_METHOD(
    fj_io_thread_sleep_destroy_manager, enum fj_error, struct fj_io_thread_sleep_manager *manager)

/**
    \param opt_duration If NULL, the thread will sleep indefinitely until any events arrive.
*/
FJ_METHOD(
    fj_io_thread_sleep,
    enum fj_error,
    struct fj_io_thread_sleep_manager *manager,
    struct fj_io_thread *io_thread,
    fj_time const *opt_duration)

FJ_METHOD_LIST_BEGIN(fj_io_thread_sleep)
FJ_METHOD_LIST_ITEM(fj_io_thread_sleep_create_manager)
FJ_METHOD_LIST_ITEM(fj_io_thread_sleep_destroy_manager)
FJ_METHOD_LIST_ITEM(fj_io_thread_sleep)
FJ_METHOD_LIST_END()

#endif
