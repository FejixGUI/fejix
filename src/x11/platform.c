#include <fejix/platform.h>


fj_err fj_x11_app_dispatcher(void *, int32_t, void *, struct fj_task *);
fj_err fj_x11_timer_dispatcher(void *, int32_t, void *, struct fj_task *);
fj_err fj_x11_window_dispatcher(void *, int32_t, void *, struct fj_task *);


fj_dispatcher get_dispatcher_(enum fj_type type)
{
    switch (type) {
        case FJ_APP:
            return fj_x11_app_dispatcher;

        default:
            return NULL;
    }
}

struct fj_platform fj_x11_platform = {
    .name = "x11",
    .version = { FJ_VERSION_MAJOR, FJ_VERSION_MINOR, FJ_VERSION_PATCH },
    .get_dispatcher = get_dispatcher_,
};
