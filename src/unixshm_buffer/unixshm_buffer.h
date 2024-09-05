#ifndef FEJIX_INTERNAL_UNIXSHM_H_
#define FEJIX_INTERNAL_UNIXSHM_H_


#include <fejix/core/base.h>


/** Growable shared-memory buffer with length of powers of two. */
struct fj_unixshm_buffer {
    int32_t file;
    size_t size;
    void * data;
};


fj_err_t fj_unixshm_buffer_create(struct fj_unixshm_buffer /*out*/ * buffer, size_t size);

fj_err_t fj_unixshm_buffer_destroy(struct fj_unixshm_buffer * buffer);

/** Shrinking the buffer is not permitted. */
fj_err_t fj_unixshm_buffer_resize(struct fj_unixshm_buffer * buffer, size_t size);

void * fj_unixshm_buffer_get_data(struct fj_unixshm_buffer * buffer);


#endif
