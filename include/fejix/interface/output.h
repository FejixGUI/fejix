#ifndef FEJIX_INTERFACE_OUTPUT_H_
#define FEJIX_INTERFACE_OUTPUT_H_


#include <fejix/interface/client.h>


struct fj_canvas { void * user_data; };
struct fj_output { void * user_data; };


typedef uint32_t fj_output_flags_t;

enum fj_output_flags {
    FJ_OUTPUT_HIDDEN = (1<<0),
    FJ_OUTPUT_ASYNC = (1<<1),
    FJ_OUTPUT_SYNC = (1<<2),
};

typedef uint32_t fj_output_update_flags_t;

enum fj_output_update_flags {
    FJ_OUTPUT_UPDATE_PRESENT = (1<<0),
    FJ_OUTPUT_UPDATE_FLAGS = (1<<1),
};


struct fj_output_caps {
    fj_output_flags_t flags;
};

struct fj_output_info {
    fj_output_flags_t flags;
};

struct fj_output_callbacks {
    fj_err_t (* init)(void */*?*/ data, struct fj_output_caps const */*?*/ caps);

    fj_err_t (* present)(void */*?*/ data, struct fj_output * output);
};

struct fj_output_iface {
    fj_err_t (* init)(
        struct fj_client * client,
        struct fj_output_callbacks const * callbacks
    );

    fj_err_t (* deinit)(struct fj_client * client);

    fj_err_t (* create)(
        struct fj_client * client,
        struct fj_output */*? out*/ * output,
        struct fj_canvas * canvas,
        struct fj_output_info const * output_info
    );

    fj_err_t (* destroy)(struct fj_client * client, struct fj_output * output);

    fj_err_t (* update)(
        struct fj_client * client,
        struct fj_output * output,
        struct fj_output_info const * output_info,
        fj_output_update_flags_t update_flags
    );
};


#endif
