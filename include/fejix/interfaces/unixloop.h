#ifndef FEJIX_UNIXLOOP_H_
#define FEJIX_UNIXLOOP_H_


#include <fejix/fejix.h>


// TODO
struct FJ_INTERFACE(fj_unixloop) {
    fj_err_t FJ_METHOD(add_watch)(
        struct fj_sys * sys,
        int32_t file_descriptor
    );

    fj_err_t FJ_METHOD(remove_watch)(
        struct fj_sys * sys,
        int32_t file_descriptor
    );
};


#endif
