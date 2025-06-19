#ifndef FEJIX_DISPLAY_H_INCLUDED
#define FEJIX_DISPLAY_H_INCLUDED


#include <fejix/shell.h>


enum fj_display_event_type {
    FJ_DISPLAY_EVENT_ADD,
    FJ_DISPLAY_EVENT_REMOVE,
    FJ_DISPLAY_EVENT_SIZE_CHANGE,
    FJ_DISPLAY_EVENT_DENSITY_CHANGE,
};

struct fj_display_size_change_event {
    struct fj_size2d size;
};

union fj_display_event {
    struct fj_display_size_change_event const *size_change;
};

enum fj_display_request_type {
    FJ_DISPLAY_REQUEST_SIZE_HINT,
};

struct fj_display_size_hint_request {
    struct fj_size2d size_hint;
};

union fj_display_request {
    struct fj_size2d *size;
};


FJ_HANDLE(fj_display)
FJ_HANDLE(fj_display_token)


typedef enum fj_status (*fj_display_listener)(
    struct fj_display *display, enum fj_display_event_type type, union fj_display_event event);


FJ_PUBLIC
enum fj_status (*fj_display_set_listener)(struct fj_shell *shell, fj_display_listener listener);

FJ_PUBLIC
enum fj_status (*fj_display_get_token)(
    struct fj_display *device, struct fj_display_token **out_token);

FJ_PUBLIC
enum fj_status (*fj_display_release_token)(
    struct fj_display *device, struct fj_display_token *token);

FJ_PUBLIC
enum fj_status (*fj_display_request)(
    struct fj_display_token *context,
    enum fj_display_request_type type,
    union fj_display_request request);

FJ_PUBLIC
enum fj_status (*fj_display_flush)(struct fj_display_token *context);


#endif
