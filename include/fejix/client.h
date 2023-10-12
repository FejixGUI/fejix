#ifndef FEJIX_CLIENT_H_
#define FEJIX_CLIENT_H_


#include <fejix/base.h>


struct fj_launch_info {
    fj_string_t client_id;

    fj_err_t (* init)(
        void * user_data,
        void * client_data,
        struct fj_client_interface * client,
        struct fj_client_feature_info * features,
        uint32_t feature_count
    );
};


struct fj_launcher {
    fj_string_t platform_name;

    fj_err_t (* launch)(
        void * user_data,
        struct fj_launch_info * info
    );
};


fj_string_t fj_get_selected_platform_name(void);

struct fj_launcher const * fj_get_launchers(void);


#endif
