#ifndef FEJIX_CLIENT_H_
#define FEJIX_CLIENT_H_


#include <fejix/base.h>


typedef uint32_t fj_run_type_t;

enum fj_run_type_values {
    /** Represents most kinds of main program functions. */
    FJ_RUN_TYPE_MAIN,
};

typedef uint32_t fj_class_id_t;

enum fj_class_id_values {
    FJ_CLASS_WINDOW // TODO
};

typedef uint32_t fj_property_id_t;

enum fj_property_id_values {
    FJ_PROP_WINDOW_SIZE // TODO
};

typedef uint32_t fj_property_event_t;

enum fj_property_event_values {
    FJ_PROPERTY_REQUESTED,
    FJ_PROPERTY_SET,
};

typedef fj_err_t (fj_property_requestor_fn_t)(
    void * client,
    void * object,
    void const * FJ_NULLABLE property_value
);

typedef fj_err_t (fj_property_listener_fn_t)(
    void * FJ_NULLABLE callback_data,
    void * object,
    void const * FJ_NULLABLE property_value,
    fj_property_event_t property_event
);

typedef void (fj_property_listener_setter_fn_t)(
    void * object,
    fj_property_listener_fn_t property_listener
);

struct fj_property_info {
    fj_property_id_t property_id;
    fj_property_requestor_fn_t * FJ_NULLABLE request;
    fj_property_listener_setter_fn_t * FJ_NULLABLE set_listener;
};

struct fj_class_info {
    void const * class_methods;
    struct fj_property_info const * FJ_ARRAY property_list;
    uint32_t property_count;
};

typedef void (fj_class_listener_fn_t)(
    void * FJ_NULLABLE callback_data,
    struct fj_class_info const * class_info
);

typedef void (fj_class_listener_setter_fn_t)(
    void * state,
    fj_class_listener_fn_t class_listener
);

struct fj_class_init_info {
    fj_class_id_t class_id;
    fj_class_listener_setter_fn_t * set_listener;
};

struct fj_gprotocol_info {
    /** Gets the list of class initialisers.

        To initialise a class, you need to set the class listener.

        The returned array is ordered by the class id. */
    void (* get_class_init_list)(
        struct fj_class_init_info const * FJ_ARRAY * FJ_OUT class_init_list,
        uint32_t * FJ_OUT class_init_count
    );

    fj_err_t (* create_state)(
        void * FJ_NULLABLE * FJ_OUT state
    );

    void (* destroy_state)(
        void * state
    );

    void (* set_callback_data)(
        void * state,
        void * FJ_NULLABLE callback_data
    );

    fj_err_t (* run)(
        void * state,
        fj_run_type_t run_type,
        void * FJ_NULLABLE run_info
    );
};

/** Returns the name of the gprotocol that the program should try to use.

    First, it tries to read the `FEJIX_GPROTOCOL` environment variable.
    If that fails, it tries to read `XDG_SESSION_TYPE`.
    If that fails, it returns the name of the first gprotocol on the list. */
fj_string_t fj_gprotocol_get_hint(void);

void fj_gprotocol_get_list(
    struct fj_gprotocol_info const * FJ_ARRAY * FJ_OUT gprotocol_list,
    uint32_t * FJ_OUT gprotocol_count
);


#endif
