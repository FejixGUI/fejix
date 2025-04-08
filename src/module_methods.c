#ifdef FJ_BUILDING_SINGLE_BACKEND

#    define FJ_METHOD(NAME, RETURN_TYPE, ...)                    \
        FJ_PUBLIC RETURN_TYPE (*NAME)(__VA_ARGS__);              \
                                                                 \
        RETURN_TYPE NAME##FJ_SINGLE_BACKEND_SUFFIX(__VA_ARGS__); \
                                                                 \
        RETURN_TYPE (*NAME)(__VA_ARGS__) = NAME##FJ_SINGLE_BACKEND_SUFFIX;

#else

#    define FJ_METHOD(NAME, RETURN_TYPE, ...)       \
        FJ_PUBLIC RETURN_TYPE (*NAME)(__VA_ARGS__); \
                                                    \
        RETURN_TYPE (*NAME)(__VA_ARGS__) = NULL;

#    define FJ_METHOD_NONNULL(NAME, RETURN_TYPE, ...)  \
        FJ_PUBLIC RETURN_TYPE (*NAME)(__VA_ARGS__);    \
                                                       \
        static RETURN_TYPE NAME##_default(__VA_ARGS__) \
        {                                              \
            return FJ_ERR_UNIMPLEMENTED;               \
        }                                              \
                                                       \
        RETURN_TYPE (*NAME)(__VA_ARGS__) = NAME##_default;

#endif


// Suppress warnings about unused arguments in default dummies for non-null methods.

// This applies to both GCC and Clang
#if defined(__GNUC__)
#    pragma GCC diagnostic ignored "-Wunused"
#endif
// This applies to MSVC
#if defined(_MSC_VER)
#    pragma warning(disable : 4100)
#endif

// This is a hand-written list of all library module headers:
#include <fejix/modules/app.h>
