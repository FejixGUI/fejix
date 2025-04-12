#ifndef FEJIX_UNIXBASE_SHM_H_INCLUDED
#define FEJIX_UNIXBASE_SHM_H_INCLUDED


#include <fejix/core.h>


/** Growable shared memory chunk with size of powers of two. */
struct fj_unixbase_shm {
    int32_t file;
    size_t size;
    void *data;
};


fj_err fj_unixbase_shm_alloc(struct fj_unixbase_shm *buffer, size_t size);

fj_err fj_unixbase_shm_free(struct fj_unixbase_shm *buffer);

/** The buffer never gets shrinked. */
fj_err fj_unixbase_shm_realloc(struct fj_unixbase_shm *buffer, size_t size);

/** Closes the buffer's file descriptor, but does not unmap the memory.
    This is used when the shell still needs to use the buffer's memory, but the client handle
    is no longer needed and can be destroyed. */
fj_err fj_unixbase_shm_unref(struct fj_unixbase_shm *buffer);


#endif
