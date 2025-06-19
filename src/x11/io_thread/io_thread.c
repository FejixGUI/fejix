#include <src/x11/io_thread/io_thread.h>

#include <fejix/utils/memory.h>


enum fj_status fj_io_thread_create_manager_x11(
    fj_io_thread_event_callback event_callback, struct fj_io_thread **out_io_thread)
{
    enum fj_status e;

    e = FJ_ALLOC(out_io_thread);

    if (e)
        return e;

    e = fj_unix_events_init(&(*out_io_thread)->events, *out_io_thread);

    if (e) {
        FJ_FREE(out_io_thread);
        return e;
    }

    (*out_io_thread)->display = XOpenDisplay(NULL);

    if ((*out_io_thread)->display == NULL) {
        fj_unix_events_deinit(&(*out_io_thread)->events);
        FJ_FREE(out_io_thread);
        return FJ_STATUS_OPERATION_FAILED;
    }

    (*out_io_thread)->connection = XGetXCBConnection((*out_io_thread)->display);
    XSetEventQueueOwner((*out_io_thread)->display, XCBOwnsEventQueue);

    (*out_io_thread)->callback = event_callback;

    return FJ_STATUS_OK;
}


enum fj_status fj_io_thread_destroy_manager_x11(struct fj_io_thread *io_thread)
{
    if (XCloseDisplay(io_thread->display) != 0) {
        fj_unix_events_deinit(&io_thread->events);
        FJ_FREE(&io_thread);
        return FJ_STATUS_OPERATION_FAILED;
    }

    fj_unix_events_deinit(&io_thread->events);
    FJ_FREE(&io_thread);

    return FJ_STATUS_OK;
}


enum fj_status fj_io_thread_launched_x11(struct fj_io_thread *io_thread)
{
    while (!io_thread->finish_requested) {
        struct fj_io_thread_event event = { .type = FJ_IO_THREAD_EVENT_NO_MORE_EVENTS };
        io_thread->callback(io_thread, &event);

        fj_unix_events_wait(&io_thread->events, NULL);
    }

    struct fj_io_thread_event event = { .type = FJ_IO_THREAD_EVENT_FINISH };
    io_thread->callback(io_thread, &event);

    return FJ_STATUS_OK;
}


enum fj_status fj_io_thread_finished_x11(struct fj_io_thread *io_thread)
{
    io_thread->finish_requested = true;
    return FJ_STATUS_OK;
}


enum fj_status fj_io_thread_request_idle_x11(struct fj_io_thread *io_thread)
{
    return fj_unix_events_wakeup(&io_thread->events);
}
