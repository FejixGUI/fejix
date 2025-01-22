#ifndef FEJIX_CORE_BASE_H_
#define FEJIX_CORE_BASE_H_


#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
#    define FJ_PUBLIC_C_LINKAGE extern "C"
#else
#    define FJ_PUBLIC_C_LINKAGE
#endif

#if defined(_WIN32) && defined(FJ_OPT_INTERNAL)
#    define FJ_PUBLIC_VISIBILITY __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define FJ_PUBLIC_VISIBILITY __attribute__((visibility("default")))
#else
#    define FJ_PUBLIC_VISIBILITY
#endif

/**
    Attribute that marks public library functions.
    This typically means exporting function symbols when building a shared library.
    This also adds ``extern "C"`` for C++.
*/
#define FJ_PUBLIC FJ_PUBLIC_C_LINKAGE FJ_PUBLIC_VISIBILITY


/**
    Evaluates to an rvalue being the userdata pointer.
    The userdata pointer must always be the first member of the struct that has it.

    This is type-unsafe! Use it only on types that support userdata creation parameter.
*/
#define FJ_USERDATA(OBJECT) (*(void **) (OBJECT))

/** The length of a metric inch in metres. */
#define FJ_INCH_LENGTH (0.0025)


/** Error code. */
typedef uint32_t fj_err_t;

/** Error codes. */
enum fj_err {
    /** */
    FJ_OK,
    /** */
    FJ_ERR_UNKNOWN,
    /** */
    FJ_ERR_UNSUPPORTED,
    /** */
    FJ_ERR_NOT_FOUND,
    /** */
    FJ_ERR_CANNOT_SELECT_IMPLEMENTATION,
    /** */
    FJ_ERR_ALLOCATION_FAILED,
    /** */
    FJ_ERR_INVALID_ALLOCATION,
    /** */
    FJ_ERR_CONNECTION_FAILED,
    /** */
    FJ_ERR_REQUEST_SENDING_FAILED,
    /** */
    FJ_ERR_REQUEST_FAILED,
    /** */
    FJ_ERR_EVENT_WAITING_FAILED,
    /** */
    FJ_ERR_EVENT_READING_FAILED,
    /** */
    FJ_ERR_INVALID_TEXT_ENCODING,
    /** */
    FJ_ERR_TEMP_FILE_CREATION_FAILED,
    /** */
    FJ_ERR_SHARED_MEMORY_ALLOCATION_FAILED,

    /** */
    FJ_ERR_PREDEFEIND_COUNT,

    /** User-defined errors should begin with this number */
    FJ_ERR_USER = 0x1000,
};


typedef uint32_t fj_orientation_type_t;

enum fj_orientation_type {
    FJ_ORIENTATION_NORMAL = 0,
    /** Vertical flip (along the Y axis). */
    FJ_ORIENTATION_VFLIP = (1 << 0),
    /** Horizontal flip (along the X axis). */
    FJ_ORIENTATION_HFLIP = (1 << 1),
    /** Clock-wise 90 degree rotation. */
    FJ_ORIENTATION_ROTATE90 = (1 << 2),

    FJ_ORIENTATION_ROTATE180 = FJ_ORIENTATION_HFLIP ^ FJ_ORIENTATION_VFLIP,
    FJ_ORIENTATION_ROTATE270 = FJ_ORIENTATION_ROTATE180 ^ FJ_ORIENTATION_ROTATE90,
    FJ_ORIENTATION_ROTATE90_VFLIP = FJ_ORIENTATION_ROTATE90 ^ FJ_ORIENTATION_VFLIP,
    FJ_ORIENTATION_ROTATE180_VFLIP = FJ_ORIENTATION_ROTATE180 ^ FJ_ORIENTATION_VFLIP,
    FJ_ORIENTATION_ROTATE270_VFLIP = FJ_ORIENTATION_ROTATE270 ^ FJ_ORIENTATION_VFLIP,
};


/** Time in seconds. Timeouts typically use values from ``0..+inf``. */
typedef double fj_seconds_t;

/**
    Dot density in DPM (dots-per-metre).

    To calculate DPI (dots-per-inch), multiply by ``FJ_INCH_LENGTH``.
    The legacy basic "normal" DPI is 96 pixels per inch.
    Concepts like text/interface scaling factor is often derived from the ratio to that value.
    That is, if the current DPI is 120, the content of the appriate size is considered to be
    scaled by 120 / 96 = 125% compared to the "unscaled" ("density-unaware") content rendered at
    96 DPI.
*/
typedef double fj_dots_per_metre_t;


struct fj_position {
    uint32_t x;
    uint32_t y;
};

struct fj_relative_position {
    int32_t x;
    int32_t y;
};

struct fj_size {
    uint32_t width;
    uint32_t height;
};

struct fj_rect {
    struct fj_position position;
    struct fj_size size;
};

struct fj_relative_rect {
    struct fj_relative_position relative_position;
    struct fj_size size;
};

struct fj_version {
    uint16_t major;
    uint16_t minor;
    uint16_t patch;
};


/** Always returns a valid printable string, even for invalid or unknown error IDs. */
FJ_PUBLIC
char const *fj_err_get_description(fj_err_t error);


#endif
