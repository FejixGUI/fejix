#include <fejix/platform.h>


fj_err fj_x11_app_sender(void *, int32_t, void *);


fj_sender get_sender_(enum fj_object_type type)
{
    switch (type) {
        case FJ_OBJECT_TYPE_APP:
            return fj_x11_app_sender;

        default:
            return NULL;
    }
}

struct fj_platform fj_x11_platform = {
    .name = "x11",
    .version = { FJ_VERSION_MAJOR, FJ_VERSION_MINOR, FJ_VERSION_PATCH },
    .get_sender = get_sender_,
};
