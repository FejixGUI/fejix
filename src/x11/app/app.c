#include <src/x11/app/app.h>

#include <fejix/utils/memory.h>


enum fj_error fj_app_create_manager_x11(
    fj_app_event_callback event_callback, struct fj_app_manager **out_app_manager)
{
    FJ_TRY (FJ_ALLOC_ZEROED(out_app_manager)) {
        return fj_result;
    }

    FJ_TRY (fj_unix_events_init(&(*out_app_manager)->events, *out_app_manager)) {
        FJ_FREE(out_app_manager);
        return fj_result;
    }

    (*out_app_manager)->display = XOpenDisplay(NULL);

    if ((*out_app_manager)->display == NULL) {
        fj_unix_events_deinit(&(*out_app_manager)->events);
        FJ_FREE(out_app_manager);
        return FJ_ERROR_OPERATION_FAILED;
    }

    (*out_app_manager)->connection = XGetXCBConnection((*out_app_manager)->display);
    XSetEventQueueOwner((*out_app_manager)->display, XCBOwnsEventQueue);

    (*out_app_manager)->callback = event_callback;

    return FJ_OK;
}


enum fj_error fj_app_destroy_manager_x11(struct fj_app_manager *app_manager)
{
    if (XCloseDisplay(app_manager->display) != 0) {
        fj_unix_events_deinit(&app_manager->events);
        FJ_FREE(&app_manager);
        return FJ_ERROR_OPERATION_FAILED;
    }

    fj_unix_events_deinit(&app_manager->events);
    FJ_FREE(&app_manager);

    return FJ_OK;
}


enum fj_error fj_app_launched_x11(struct fj_app_manager *app_manager)
{
    while (!app_manager->finish_requested) {
        app_manager->callback(app_manager, FJ_APP_EVENT_IDLE, NULL);

        fj_unix_events_wait(&app_manager->events, NULL);
    }

    app_manager->callback(app_manager, FJ_APP_EVENT_FINISH, NULL);

    return FJ_OK;
}


enum fj_error fj_app_finished_x11(struct fj_app_manager *app_manager)
{
    app_manager->finish_requested = true;
    return FJ_OK;
}


enum fj_error fj_app_request_idle_x11(struct fj_app_manager *app_manager)
{
    return fj_unix_events_wakeup(&app_manager->events);
}
