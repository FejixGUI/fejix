#ifndef FEJIX_MAIN_H_INCLUDED
#define FEJIX_MAIN_H_INCLUDED


#include <fejix/core.h>


enum fj_main_alert_type {
    FJ_MAIN_ALERT_PAUSE = 0,
    FJ_MAIN_ALERT_HIBERNATE = 1,
    FJ_MAIN_ALERT_LOW_MEMORY = 2,

    FJ_MAIN_ALERT_ENUM32 = INT32_MAX,
};

enum fj_module_id {
    FJ_MODULE_CLOCK = 0,
    FJ_MODULE_WINDOW = 1,
    FJ_MODULE_RAM = 20,
    FJ_MODULE_OPENGL = 21,

    FJ_MODULE_ENUM32 = INT32_MAX,
};


/** Main module handles the event loop and provides methods of other modules. */
FJ_DEFINE_MODULE(fj_main_module)


struct fj_main_module_callbacks {
    fj_err (*finish)(void *data);

    fj_err (*idle)(void *data);

    fj_err (*alert)(void *data, enum fj_main_alert_type alert);
};


struct fj_main_module_methods {
    char const *(*get_implementation_name)(void);

    struct fj_version (*get_api_version)(void);

    fj_err (*get_module)(struct fj_main_module **out_mod);

    fj_err (*release_module)(struct fj_main_module *mod);

    void const *(*get_module_methods)(struct fj_main_module *mod, enum fj_module_id id);

    void (*set_callbacks)(
        struct fj_main_module *mod,
        struct fj_main_module_callbacks const *callbacks,
        size_t callbacks_size,
        void *callback_data);

    fj_err (*launched)(struct fj_main_module *mod);

    fj_err (*request_finish)(struct fj_main_module *mod);

    fj_err (*request_idle)(struct fj_main_module *mod);
};


FJ_PUBLIC
void fj_main_get_implementations(
    struct fj_main_module_methods const *const *out_implementations,
    uint32_t *out_implementations_length);

FJ_PUBLIC
char const *fj_main_get_default_implementation_name(void);

FJ_PUBLIC
fj_err fj_main_get_module(struct fj_main_module **out_mod);


#include <fejix/modules/helpers/main.h>


#endif
