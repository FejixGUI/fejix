#ifndef FEJIX_CLIENT_H_
#define FEJIX_CLIENT_H_


#include <fejix/base.h>


typedef uint32_t fj_interface_id_t;

enum fj_interface_id_values {
    FJ_IID_SCHEDULE = 1,
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
    fj_interface_id_t interface_id;
    fj_bool_t request_init;
};


struct fj_interface_info {
    void const * interface;
    fj_interface_id_t interface_id;
};


struct fj_client_config_info {
    /* These are the configs of the interfaces built into the library.
        The interfaces may not be actually initialised in the future
        because they may not be supported by the shell.
        Therefore, request all the interface that you may need to use. */
    struct fj_interface_config_info * FJ_NULLABLE FJ_ARRAY interface_configs;

    uint32_t interface_config_count;
};


struct fj_client_info {
    void * client_data;

    struct fj_interface_info const * FJ_NULLABLE FJ_ARRAY interfaces;

    uint32_t interface_count;
};


struct fj_launch_info {
    fj_err_t (* on_config)(
        void * FJ_NULLABLE user_data,
        struct fj_client_config_info const * config_info
    );

    fj_err_t (* on_init)(
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
