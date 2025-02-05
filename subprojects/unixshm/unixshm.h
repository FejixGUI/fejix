#ifndef FEJIX_UNIXSHM_H_
#define FEJIX_UNIXSHM_H_


#include <fejix/core/base.h>


/** Growable shared memory chunk with size of powers of two. */
struct fj_unixshm {
    int32_t file;
    size_t size;
    void *data;
};


fj_err_t fj_unixshm_alloc(struct fj_unixshm *buffer, size_t size);

fj_err_t fj_unixshm_free(struct fj_unixshm *buffer);

/** The buffer never gets shrinked. */
fj_err_t fj_unixshm_realloc(struct fj_unixshm *buffer, size_t size);

/** Closes the buffer's file descriptor, but does not unmap the memory.
    This is used when the shell still needs to use the buffer's memory, but the client handle
    is no longer needed and can be destroyed. */
fj_err_t fj_unixshm_unref(struct fj_unixshm *buffer);


#endif
