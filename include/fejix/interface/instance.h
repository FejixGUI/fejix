#ifndef FEJIX_INSTANCE_H_
#define FEJIX_INSTANCE_H_


#include <fejix/base.h>
#include <fejix/implementation.h>


struct fj_instance_info {
    /** In format of "com.some_organisation.some_program.some_module".
        This will be automatically converted to the native format. */
    uint8_t const *fjARRAY program_name;
};

struct fj_instance {
    fj_err_t (* create)(
        struct fj_implementation const * implementation,
        struct fj_instance_info const * info,
        void * fjOUT * instance_context
    );

    fj_err_t (* destroy)(
        void * instance_context
    );
};


#endif
