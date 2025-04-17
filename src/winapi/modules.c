#define FJ_INTERNAL_OPT_BACKEND_SUFFIX winapi
#include <src/modules/begin.h>

#include <fejix/modules/app.h>
#include <fejix/modules/clock.h>

FJ_MODULE_LIST_BEGIN()
FJ_MODULE_LIST_ITEM(fj_app)
FJ_MODULE_LIST_ITEM(fj_clock)
FJ_MODULE_LIST_END()

#include <src/modules/end.h>
