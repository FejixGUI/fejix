#ifndef FEJIX_UNIX_EVENTS_H_INCLUDED
#define FEJIX_UNIX_EVENTS_H_INCLUDED


#include <fejix/core.h>
#include <fejix/utils/vector.h>

#include <sys/poll.h>


typedef enum fj_error (*fj_unix_events_callback)(
    void *callback_data, int file_descriptor, short event_mask);


FJ_VECTOR_OF(fj_unix_events_pollfd_vector, struct pollfd)
FJ_VECTOR_OF(fj_unix_events_callback_vector, fj_unix_events_callback)

struct fj_unix_events {
    int wakeup_pipe[2];
    struct fj_unix_events_pollfd_vector pollfds;
    struct fj_unix_events_callback_vector callbacks;
    void *callback_data;
};


enum fj_error fj_unix_events_init(struct fj_unix_events *events, void *callback_data);

void fj_unix_events_deinit(struct fj_unix_events *events);

enum fj_error fj_unix_events_add(
    struct fj_unix_events *events,
    int file_descriptor,
    short events_to_watch,
    fj_unix_events_callback callback);

/**
    Ensures that the file descriptor is not being watched.
    If the file descriptor has not beed added to the watching list, this
    returns `FJ_OK`.
*/
enum fj_error fj_unix_events_remove(struct fj_unix_events *events, int file_descriptor);

enum fj_error fj_unix_events_wait(struct fj_unix_events *events, fj_time *opt_timeout);

enum fj_error fj_unix_events_wakeup(struct fj_unix_events *events);


#endif
