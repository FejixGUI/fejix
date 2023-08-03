#ifndef FEJIX_MALLOC_H_
#define FEJIX_MALLOC_H_


#include <fejix/core/base.h>


/// Allocates an uninitialized block of memory.
/// Works like `malloc`, but returns NULL if `size` is 0.
fj_ptr_t fj_alloc_uninit(size_t size);

/// Allocates a block of memory initialized with zeros.
/// Works like `calloc(1,)`, but returns NULL if `size` is 0.
fj_ptr_t fj_alloc_zeroed(size_t size);

/// Works like `free`. If `ptr` is NULL, the behavior is undefined.
void fj_free(fj_ptr_t ptr);

/// * If both `element_count` and `element_size` are greater than 0:
///     - If `ptr` is not NULL:
///         + reallocates the block, returns a new block
///     - If `ptr` is NULL:
///         + calls `fj_alloc_uninit`
///  * If either `element_count` or `element_size` is 0:
///     - If `ptr` is not NULL:
///         + calls `fj_free`, returns NULL
///     - If `ptr` is NULL:
///         + does nothing, returns NULL
///
/// Works similarly to `realloc`, but with distinct `element_count` and
/// `element_size`.
/// Like `realloc`, this does NOT free the old block if fails to allocate
/// a new one.
fj_ptr_t fj_realloc(fj_ptr_t ptr, uint32_t element_count, size_t element_size);


#endif
