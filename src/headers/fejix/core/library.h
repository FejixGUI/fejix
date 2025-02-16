#ifndef FEJIX_LIBRARY_INCLUDED
#define FEJIX_LIBRARY_INCLUDED


#include <fejix/core/base.h>


typedef uint32_t fj_library_load_flags_t;

enum {
    /**
        If not used, the paths use UTF-8 encoding and may be converted to the native system
        encoding.

        If used, allows specifying the path in the system text encoding directly.

        On Windows the native encoding is UTF-16 and the real type of paths is ``wchar_t const*``
        (or equivalent).

        The rationale for this flag is that loading a library can fail when loading from a path
        that contains invalid UTF-16 (which is easy to do on Windows if you retrieve the path
        from an environment variable) that may fail to correctly convert to UTF-8.
    */
    FJ_LIBRARY_SYSTEM_ENCODED_PATH = 1 << 0,

    /**
        Same as ``RTLD_GLOBAL`` of ``dlopen()``.
        If not specified, the ``dlopen()`` function uses ``RTLD_LOCAL`` by default.
    */
    FJ_LIBRARY_LOAD_GLOBAL = 1 << 1,

    /** Same as ``RTLD_LAZY`` of ``dlopen()``. If not specified, ``RTLD_NOW`` is used. */
    FJ_LIBRARY_LOAD_LAZY = 1 << 2,
};


/** Convertible to the actual function type. */
typedef void (*fj_library_function_t)(void);

/** Represents a dynamically loaded library (DLL) a.k.a. a shared object (SO) file. */
struct fj_library {
    void *library_data;
    fj_library_function_t (*load_function)(void *library_data, char const *function_name);
};


/** Ignores unsupported flags. */
FJ_PUBLIC
fj_err_t fj_library_load(
    struct fj_library *out_library, char const *path, fj_library_load_flags_t flags);

/**
    This must be called on the ``library`` if only if it was created by ``fj_library_load()``.

    Library structures may be created by the user to emulate function loading from other sources
    than dynamically loaded libraries. This function may thus be not applicable for user-created
    libraries.
*/
FJ_PUBLIC
void fj_library_unload(struct fj_library *library);


#endif
