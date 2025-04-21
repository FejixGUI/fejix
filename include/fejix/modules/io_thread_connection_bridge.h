/**
    \file
*/

#ifndef FEJIX_IO_THREAD_CONNECTION_BRIDGE_H_INCLUDED
#define FEJIX_IO_THREAD_CONNECTION_BRIDGE_H_INCLUDED


#include <fejix/modules/io_thread.h>


FJ_METHOD_WITH_FALLBACK(
    fj_io_thread_connection_bridge_get_data, uintptr_t, 0, struct fj_io_thread *io_thread)


FJ_METHOD_LIST_BEGIN(fj_io_thread_connection_bridge)
FJ_METHOD_LIST_ITEM(fj_io_thread_connection_bridge_get_data)
FJ_METHOD_LIST_END()


#endif
