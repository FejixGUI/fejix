#ifndef FEJIX_INSTANCE_H_
#define FEJIX_INSTANCE_H_


#include <fejix/implementation.h>


FJ_DEFINE_UNIQUE_TYPE(fj_instance_t)


struct fj_instance_info {
    /** In format of "com.Some_Organisation.Some_Program.Some_Module".
        This will be automatically converted to the native format. */
    uint8_t const */*[]*/ program_name;
};

struct fj_instance {
    fj_err_t (* create)(
        fj_instance_t */*?*/ /*out*/ * instance,
        struct fj_implementation const * implementation,
        struct fj_instance_info const * info
    );

    fj_err_t (* destroy)(
        fj_instance_t * instance
    );
};


#endif
