#ifndef FEJIX_UNIX_SHARED_MEMORY_H_
#define FEJIX_UNIX_SHARED_MEMORY_H_


#include <fejix/base.h>


/** Growable shared memory chunk with size of powers of two. */
struct fj_unix_shared_buffer {
    int32_t file;
    size_t size;
    void *data;
};


enum fj_status fj_unix_shared_alloc(struct fj_unix_shared_buffer *buffer, size_t size);

enum fj_status fj_unix_shared_free(struct fj_unix_shared_buffer *buffer);

/** The buffer never gets shrinked. */
enum fj_status fj_unix_shared_realloc(struct fj_unix_shared_buffer *buffer, size_t size);

/** Closes the buffer's file descriptor, but does not unmap the memory.
    This is used when the shell still needs to use the buffer's memory, but the client handle
    is no longer needed and can be destroyed. */
enum fj_status fj_unix_shared_unref(struct fj_unix_shared_buffer *buffer);


#endif
