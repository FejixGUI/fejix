#ifndef FEJIX_WINDOW_H_
#define FEJIX_WINDOW_H_


#include <fejix/app.h>


FJ_SERVICE(fj_window_service)
FJ_OBJECT(fj_window)


FJ_API(fj_window_service_new, struct fj_window_service **out_service, struct fj_app *app)

FJ_API(fj_window_service_del, struct fj_window_service *service)

FJ_API(fj_window_new, struct fj_window **out_window, struct fj_window_service *service)

FJ_API(fj_window_del, struct fj_window *window)

/**
    This function ensures that all recent changes to the window are sent to the system and
    will come into effect.

    Windows have visible state and because changing parts of the state independently may sometimes
    lead to visual defects some platforms (like Wayland) implement state caching so that it can
    be atomically committed.
*/
FJ_API(fj_window_commit, struct fj_window *window)

FJ_API(fj_window_set_preferred_size, struct fj_window *window, struct fj_size2d const *size)

FJ_API(fj_window_set_title, struct fj_window *window, char const *title)

FJ_API(fj_window_request_update, struct fj_window *window)

FJ_API_VOID(fj_window_update_callback, struct fj_window *window)

FJ_API_VOID(fj_window_close_callback, struct fj_window *window)

FJ_API_VOID(fj_window_size_changed_callback, struct fj_window *window)

FJ_API_VOID(fj_window_orientation_changed_callback, struct fj_window *window)

FJ_API_VOID(fj_window_density_changed_callback, struct fj_window *window)


#endif
