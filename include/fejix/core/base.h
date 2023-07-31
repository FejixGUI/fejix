#ifndef FEJIX_BASE_H_
#define FEJIX_BASE_H_


#include <stdint.h>
#include <stddef.h>


/// Identifier.
typedef uint32_t fj_id_t;
/// Void pointer.
typedef void * fj_ptr_t;
/// UTF-8 string for most use cases.
typedef char * fj_string_t;
/// Constant ASCII string for error messages.
typedef const char * fj_err_t;


// Predefined errors for avoiding duplication:

/// Success, equal to NULL.
extern fj_err_t fj_ok;
/// Memory allocation error.
extern fj_err_t fj_err_malloc;


#endif