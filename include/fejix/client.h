#ifndef FEJIX_CLIENT_H_
#define FEJIX_CLIENT_H_


#include <fejix/base.h>


typedef uint32_t fj_interface_id_t;

enum fj_interface_id_values {
    FJ_CLIENT_RUN_ID = 1,
    FJ_CLIENT_SCHEDULE_ID,
};


typedef uint32_t fj_launcher_flags_t;

enum fj_launcher_flags_values {
    /* `launch()` actually runs the program.
        When it returns, the program entrypoint should return. */
    FJ_LAUNCHER_BLOCKING = (1<<0),

    /* `launch()` function only sets the launch info for the launcher.
        After it returns, the library entrypoint should return for the launcher
        to run. */
    FJ_LAUNCHER_DEFERRED = (1<<1),
};


struct fj_client_interface_listener {
    fj_err_t (* FJ_CALLBACK init)(
        void * FJ_NULLABLE user_data,
        void * interface
    );

    fj_interface_id_t interface_id;
};


struct fj_client_info {
    void * client_data;
    struct fj_client_interface_listener * FJ_ARRAY client_interface_listeners;
    uint32_t client_interface_count;
};


struct fj_launch_info {
    fj_err_t (* FJ_CALLBACK init)(
        void * FJ_NULLABLE user_data,
        struct fj_client_info const * client_info
    );

    void * FJ_NULLABLE user_data;
};


struct fj_launcher {
    fj_err_t (* launch)(struct fj_launch_info const * info);

    fj_string_t name;
    fj_launcher_flags_t flags;
};


fj_string_t fj_get_selected_launcher_name(void);

struct fj_launcher const * FJ_ARRAY fj_get_launchers(void);


#endif
