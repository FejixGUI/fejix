#ifndef FEJIX_CLIENT_H_
#define FEJIX_CLIENT_H_


#include <fejix/base.h>


typedef uint32_t fj_interface_id_t;

enum fj_interface_id_values {
    FJ_IID_CLIENT_DATA = 0,
    FJ_IID_SCHEDULE,
};


typedef uint32_t fj_launcher_flags_t;

enum fj_launcher_flags_values {
    /* `launch()` actually runs the program.
        When it returns, the program entrypoint should return. */
    FJ_LAUNCHER_BLOCKING = (1<<0),

    /* When present, if the launcher is blocking, identifies that the launcher
        can be launched multiple times. */
    FJ_LAUNCHER_REUSABLE = (1<<1),

    /* `launch()` function only sets the launch info for the launcher.
        After it returns, the library entrypoint should return for the launcher
        to run. */
    FJ_LAUNCHER_DEFERRED = (1<<2),
};


struct fj_interface_config_info {
    void * FJ_NULLABLE interface_listener;
    fj_interface_id_t interface_id;
};


struct fj_launch_info {
    /* Called during the launch before the client gets initialised. */
    fj_err_t (* on_launch)(
        void * FJ_NULLABLE user_data,
        struct fj_interface_config_info * FJ_ARRAY configs
    );

    /* Called during the unlaunch after the client gets deinitialised. */
    fj_err_t (* on_unlaunch)(
        void * FJ_NULLABLE user_data,
        fj_err_t run_error
    );

    void * FJ_NULLABLE user_data;
};


struct fj_launcher {
    fj_err_t (* launch)(struct fj_launch_info const * info);

    fj_string_t name;
    fj_launcher_flags_t flags;
};


struct fj_client_data_listener {
    fj_err_t (* on_init)(
        void * FJ_NULLABLE user_data,
        void * client_data
    );
};


fj_string_t fj_get_selected_launcher_name(void);

struct fj_launcher const * FJ_ARRAY fj_get_launchers(void);


#endif
