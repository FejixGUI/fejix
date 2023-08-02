#ifndef FEJIX_UNIXLOOP_H_
#define FEJIX_UNIXLOOP_H_


#include <fejix/fejix.h>


// TODO
FJ_DEFINE_INTERFACE(fj_unixloop) {
    FJ_DEFINE_METHOD(add_watch, fj_err_t, (
        struct fj_sys * sys,
        int32_t file_descriptor
    ))

    FJ_DEFINE_METHOD(remove_watch, fj_err_t, (
        struct fj_sys * sys,
        int32_t file_descriptor
    ))
};


#endif
