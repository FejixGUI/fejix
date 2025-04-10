#ifndef FJ_BACKEND_SUFFIX
#    error You need to #define FJ_BACKEND_SUFFIX
#endif

#define FJ_CONCAT_(A, B) A##B
#define FJ_CONCAT(A, B) FJ_CONCAT_(A, B)
#define FJ_SUFFIXED(NAME) FJ_CONCAT(NAME##_, FJ_BACKEND_SUFFIX)

#ifdef FJ_BUILDING_SINGLE_BACKEND

#    define FJ_METHOD(NAME, RETURN_TYPE, ...)       \
        FJ_PUBLIC RETURN_TYPE (*NAME)(__VA_ARGS__); \
        RETURN_TYPE FJ_SUFFIXED(NAME)(__VA_ARGS__); \
        RETURN_TYPE (*NAME)(__VA_ARGS__) = FJ_SUFFIXED(NAME);

#    define FJ_METHOD_NONNULL(NAME, RETURN_TYPE, ...) FJ_METHOD(NAME, RETURN_TYPE, __VA_ARGS__)

#    define FJ_METHOD_LIST_BEGIN(MODULE_NAME)
#    define FJ_METHOD_LIST_ITEM(METHOD_NAME)
#    define FJ_METHOD_LIST_END

#    define FJ_MODULE_LIST_BEGIN()
#    define FJ_MODULE_LIST_ITEM(MODULE_NAME)
#    define FJ_MODULE_LIST_END()

#else

#    define FJ_METHOD(NAME, RETURN_TYPE, ...) FJ_PUBLIC RETURN_TYPE (*NAME)(__VA_ARGS__);
#    define FJ_METHOD_NONNULL(NAME, RETURN_TYPE, ...) FJ_METHOD(NAME, RETURN_TYPE, __VA_ARGS__)

#    define FJ_METHOD_LIST_BEGIN(MODULE_NAME)                     \
        static void FJ_SUFFIXED(MODULE_NAME##_init_methods)(void) \
        {
#    define FJ_METHOD_LIST_ITEM(METHOD_NAME) METHOD_NAME = FJ_SUFFIXED(METHOD_NAME);
#    define FJ_METHOD_LIST_END() }


#    define FJ_MODULE_LIST_BEGIN()              \
        void FJ_SUFFIXED(fj_init_methods)(void) \
        {
#    define FJ_MODULE_LIST_ITEM(MODULE_NAME) FJ_SUFFIXED(MODULE_NAME##_init_methods)();
#    define FJ_MODULE_LIST_END() }

#endif
