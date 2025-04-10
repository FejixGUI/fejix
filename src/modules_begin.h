#ifndef FJ_BACKEND_SUFFIX
#    error You need to #define FJ_BACKEND_SUFFIX
#endif


#ifdef FJ_BUILDING_SINGLE_BACKEND

#    define FJ_METHOD(NAME, RETURN_TYPE, ...)             \
        FJ_PUBLIC RETURN_TYPE (*NAME)(__VA_ARGS__);       \
        RETURN_TYPE NAME##FJ_BACKEND_SUFFIX(__VA_ARGS__); \
        RETURN_TYPE (*NAME)(__VA_ARGS__) = NAME##FJ_BACKEND_SUFFIX;

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

#    define FJ_METHOD_LIST_BEGIN(MODULE_NAME)                    \
        static void MODULE_NAME##_init_##FJ_BACKEND_SUFFIX(void) \
        {
#    define FJ_METHOD_LIST_ITEM(METHOD_NAME) METHOD_NAME = METHOD_NAME##FJ_BACKEND_SUFFIX;
#    define FJ_METHOD_LIST_END() }


#    define FJ_MODULE_LIST_BEGIN()                     \
        void fj_modules_init_##FJ_BACKEND_SUFFIX(void) \
        {
#    define FJ_MODULE_LIST_ITEM(MODULE_NAME) MODULE_NAME##_init_methods_##FJ_BACKEND_SUFFIX();
#    define FJ_MODULE_LIST_END() }

#endif
