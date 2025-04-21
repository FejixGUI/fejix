#ifndef FEJIX_FJ_WINDOW_BRIDGE_H_INCLUDED
#define FEJIX_FJ_WINDOW_BRIDGE_H_INCLUDED


#include <fejix/modules/window.h>


FJ_METHOD_WITH_FALLBACK(fj_window_bridge_get_data, uintptr_t, 0, struct fj_window *window)


FJ_METHOD_LIST_BEGIN(fj_window_bridge)
FJ_METHOD_LIST_ITEM(fj_window_bridge_get_data)
FJ_METHOD_LIST_END()


#endif
