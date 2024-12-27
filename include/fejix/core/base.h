#ifndef FEJIX_CORE_BASE_H_
#define FEJIX_CORE_BASE_H_


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
#    define FJ_API_LINKAGE extern "C"
#else
#    define FJ_API_LINKAGE
#endif

#if defined(_WIN32) && defined(FJ_OPT_INTERNAL)
#    define FJ_API_VISIBILITY __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define FJ_API_VISIBILITY __attribute__((visibility("default")))
#else
#    define FJ_API_VISIBILITY
#endif

#define FJ_API FJ_API_LINKAGE FJ_API_VISIBILITY

#ifdef FJ_OPT_INTERNAL
#    define FJ_DECLARE_ABSTRACT_OBJECT(STRUCT_NAME) struct STRUCT_NAME;
#    define FJ_DEFINE_ABSTRACT_OBJECT struct fj_object object;
#else
#    define FJ_DECLARE_ABSTRACT_OBJECT(STRUCT_NAME) \
        struct STRUCT_NAME {                        \
            struct fj_object object;                \
        };
#endif


#ifdef FJ_OPT_INTERNAL
#    define FJ_RESPOND(OBJ, REQUEST_ID, REQUEST, RESPONSE) \
        (((struct fj_object *) (void *) (OBJ))->respond((OBJ), (REQUEST_ID), (REQUEST), (RESPONSE)))
#else
#    define FJ_RESPOND(OBJECT, REQUEST_ID, REQUEST, RESPONSE) \
        ((OBJECT)->object.respond((OBJECT), (REQUEST_ID), (REQUEST), (RESPONSE)))
#endif


/** Error code. */
typedef uint32_t fj_err_t;

/** Error codes. */
enum fj_err {
    FJ_OK,
    FJ_ERR_UNKNOWN,
    FJ_ERR_UNSUPPORTED,
    FJ_ERR_ALLOCATION_FAILED,
    FJ_ERR_INVALID_ALLOCATION,
    FJ_ERR_CONNECTION_FAILED,
    FJ_ERR_REQUEST_SENDING_FAILED,
    FJ_ERR_REQUEST_FAILED,
    FJ_ERR_EVENT_WAITING_FAILED,
    FJ_ERR_EVENT_READING_FAILED,
    FJ_ERR_INVALID_TEXT_ENCODING,
    FJ_ERR_INTERFACE_INIT_FAILED,
    FJ_ERR_TEMP_FILE_CREATION_FAILED,
    FJ_ERR_SHARED_MEMORY_ALLOCATION_FAILED,

    FJ_ERR_MAX = FJ_ERR_SHARED_MEMORY_ALLOCATION_FAILED,

    /** User-defined errors should begin with this number */
    FJ_ERR_USER = 0x1000,
};


/** */
typedef uint32_t fj_tag_type_t;

/** */
enum fj_tag_type {
    /** */
    FJ_TAG_U32,
    /** */
    FJ_TAG_I32,
    /** */
    FJ_TAG_U64,
    /** */
    FJ_TAG_I64,
    /** */
    FJ_TAG_UPTR,
    /** */
    FJ_TAG_IPTR,
    /** */
    FJ_TAG_PTR,
};


/**
    Tag is something that can identify an object, e.g. a pointer to the object, an ID of the
   object or any other pointer or numeric value.
*/
union fj_tag {
    /** */
    uint32_t u32;
    /** */
    int32_t i32;
    /** */
    uint64_t u64;
    /** */
    int64_t i64;
    /** */
    uintptr_t uptr;
    /** */
    intptr_t iptr;
    /** */
    void *ptr;
};


/** Use standard ``true``/``false`` for this. */
typedef uint8_t fj_bool8_t;

typedef double fj_seconds_t;


typedef uint32_t fj_request_id_t;

typedef fj_err_t (*fj_responder_t)(
    void *object,
    fj_request_id_t request_id,
    void const *request,
    void *response
);

struct fj_object {
    union fj_tag tag;
    fj_responder_t respond;
};


struct fj_version {
    uint16_t major;
    uint16_t minor;
    uint16_t patch;
};


#endif
