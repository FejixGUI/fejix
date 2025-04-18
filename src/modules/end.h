#undef FJ_METHOD
#undef FJ_METHOD_NONNULL

#undef FJ_METHOD_LIST_BEGIN
#undef FJ_METHOD_LIST_ITEM
#undef FJ_METHOD_LIST_END

#undef FJ_MODULE_LIST_BEGIN
#undef FJ_MODULE_LIST_ITEM
#undef FJ_MODULE_LIST_END

#define FJ_METHOD(NAME, RETURN_TYPE, ...)       \
    FJ_PUBLIC RETURN_TYPE (*NAME)(__VA_ARGS__); \
    RETURN_TYPE (*NAME)(__VA_ARGS__) = NULL;

#define FJ_METHOD_NONNULL(NAME, RETURN_TYPE, ...)  \
    FJ_PUBLIC RETURN_TYPE (*NAME)(__VA_ARGS__);    \
                                                   \
    static RETURN_TYPE NAME##_default(__VA_ARGS__) \
    {                                              \
        return FJ_ERROR_UNIMPLEMENTED;             \
    }                                              \
                                                   \
    RETURN_TYPE (*NAME)(__VA_ARGS__) = NAME##_default;

#define FJ_METHOD_LIST_BEGIN(MODULE_NAME)
#define FJ_METHOD_LIST_ITEM(METHOD_NAME)
#define FJ_METHOD_LIST_END()


// Suppress warnings about unused arguments in the default dummies for non-null methods.

// This applies to both GCC and Clang
#if defined(__GNUC__)
#    pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
// This applies to MSVC
#if defined(_MSC_VER)
#    pragma warning(disable : 4100)
#endif


#include <fejix/modules/_all.h>
