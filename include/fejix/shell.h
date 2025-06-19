#ifndef FEJIX_SHELL_H_INCLUDED
#define FEJIX_SHELL_H_INCLUDED


#include <fejix/core.h>


enum fj_shell_event_type {
    FJ_SHELL_DISCONNECT,
};

union fj_shell_event {
    void *_unused;
};


FJ_HANDLE(fj_shell)


typedef enum fj_status (*fj_shell_listener)(
    struct fj_shell *shell, enum fj_shell_event_type type, union fj_shell_event event);


FJ_PUBLIC
enum fj_status (*fj_shell_connect)(struct fj_shell **out_shell);

FJ_PUBLIC
enum fj_status (*fj_shell_disconnect)(struct fj_shell *shell);

FJ_PUBLIC
enum fj_status (*fj_shell_set_listener)(struct fj_shell *shell, fj_shell_listener listener);

FJ_PUBLIC
enum fj_status (*fj_shell_listen)(struct fj_shell *shell);


#endif
