#ifndef FEJIX_STATE_H_
#define FEJIX_STATE_H_


#include <fejix/base.h>


typedef uint32_t fj_state_id_t;

typedef uint32_t fj_state_flags_t;

enum fj_state_flags_values {
    /* Initialisable means that the state can be created with a custom initial
        value. */
    FJ_STATE_INITABLE  = (1<<0),

    /* Settable means that the state can be freely changed by the program. */
    FJ_STATE_SETTABLE  = (1<<1),

    /* Volatile means that the state can be freely changed outside the program
        and that the program can handle its changes. */
    FJ_STATE_VOLATILE  = (1<<2),
};


struct fj_state_info {
    fj_err_t (* FJ_NULLABLE on_init)(
        void * FJ_NULLABLE user_data
    );

    fj_err_t (* FJ_NULLABLE set)(
        void * client,
        void * FJ_NULLABLE value
    );

    fj_err_t (* FJ_NULLABLE on_change)(
        void * FJ_NULLABLE user_data,
        void * FJ_NULLABLE new_value
    );


    fj_state_id_t id;
    fj_state_flags_t flags;
};


#endif
