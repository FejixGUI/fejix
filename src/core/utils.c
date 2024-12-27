#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>

#include <string.h>


/** Contains strings and NULLs for unimplemented descriptions. */
static char const *const error_descriptions[FJ_ERR_MAX + 1] = {
    [FJ_OK] = "success",
    [FJ_ERR_UNKNOWN] = "unknown error",
    [FJ_ERR_ALLOCATION_FAILED] = "allocation failed (out of memory)",
    [FJ_ERR_INVALID_ALLOCATION] = "invalid allocation",
    [FJ_ERR_CONNECTION_FAILED] = "connection failed",
    [FJ_ERR_REQUEST_SENDING_FAILED] = "request sending failed",
    [FJ_ERR_REQUEST_FAILED] = "request failed",
    [FJ_ERR_EVENT_WAITING_FAILED] = "event waiting failed",
    [FJ_ERR_EVENT_READING_FAILED] = "event reading failed",
    [FJ_ERR_INVALID_TEXT_ENCODING] = "invalid text encoding",
    [FJ_ERR_INTERFACE_INIT_FAILED] = "interface initialisation failed",
    [FJ_ERR_TEMP_FILE_CREATION_FAILED] = "temporary file creation failed",
    [FJ_ERR_SHARED_MEMORY_ALLOCATION_FAILED] = "shared memory allocation failed",
};


char const *fj_ext_error_get_description(fj_err_t error)
{
    if (error >= FJ_ERR_USER) {
        return "user-defined error";
    }

    if (error > FJ_ERR_MAX) {
        return "invalid error ID";
    }

    if (error_descriptions[error] == NULL) {
        return "some error (description not implemented yet)";
    };

    return error_descriptions[error];
}


static uint32_t uint32_hash32(uint32_t x)
{
    x ^= x >> 16;
    x *= UINT32_C(0x45d9f3b);
    x ^= x >> 16;
    x *= UINT32_C(0x45d9f3b);
    x ^= x >> 16;
    return x;
}


static uint32_t uint64_hash32(uint64_t x)
{
    x ^= x >> 32;
    x *= UINT64_C(0xd6e8feb86659fd93);
    x ^= x >> 32;
    x *= UINT64_C(0xd6e8feb86659fd93);
    x ^= x >> 32;
    return (uint32_t) x;
}


static uint32_t uintptr_hash32(uintptr_t x)
{
    bool uintptr_fits_in_uint32 = sizeof(uintptr_t) <= sizeof(uint32_t);

    if (uintptr_fits_in_uint32) {
        return uint32_hash32((uint32_t) x);
    } else {
        return uint64_hash32((uint64_t) x);
    }
}


static uint32_t ptr_hash32(void *x)
{
    return uintptr_hash32((uintptr_t) x);
}


fj_bool8_t fj_tag_eq(union fj_tag a, union fj_tag b, fj_tag_type_t type)
{
    switch (type) {
        case FJ_TAG_U32:
            return a.u32 == b.u32;
        case FJ_TAG_I32:
            return a.i32 == b.i32;
        case FJ_TAG_U64:
            return a.u64 == b.u64;
        case FJ_TAG_I64:
            return a.i64 == b.i64;
        case FJ_TAG_UPTR:
            return a.uptr == b.uptr;
        case FJ_TAG_IPTR:
            return a.iptr == b.iptr;
        case FJ_TAG_PTR:
            return a.ptr == b.ptr;
        default:
            return false;
    }
}


uint32_t fj_tag_hash32(union fj_tag x, fj_tag_type_t type)
{
    switch (type) {
        case FJ_TAG_U32:
            return uint32_hash32(x.u32);
        case FJ_TAG_I32:
            return uint32_hash32((uint32_t) x.i32);
        case FJ_TAG_U64:
            return uint64_hash32(x.u64);
        case FJ_TAG_I64:
            return uint64_hash32((uint64_t) x.i64);
        case FJ_TAG_UPTR:
            return uintptr_hash32(x.uptr);
        case FJ_TAG_IPTR:
            return uintptr_hash32((uint64_t) x.iptr);
        case FJ_TAG_PTR:
            return ptr_hash32(x.ptr);

        default:
            return false;
    }
}


fj_bool8_t fj_str_eq(char const *a, char const *b)
{
    if (a == NULL && b == NULL) {
        return true;
    }

    if (a == NULL || b == NULL) {
        return false;
    }

    return strcmp(a, b) == 0;
}


fj_err_t fj_str_clone(char const *str, char const **clone)
{
    size_t length = strlen(str);

    FJ_TRY (FJ_REALLOC_UNINIT(clone, length)) {
        return fj_result;
    }

    memcpy((void *) *clone, (void *) str, length);

    return FJ_OK;
}


uint32_t fj_u32_next_power_of_two(uint32_t x)
{
    if (x >= UINT32_C(1) << 31) {
        return UINT32_MAX;
    }

    uint32_t y = 1;

    while (y <= x) {
        y *= 2;
    }

    return y;
}


uint32_t fj_u32_prev_power_of_two(uint32_t x)
{
    if (x == 0) {
        return 1;
    }

    uint32_t y = 1;

    while (y < x - y) {
        y *= 2;
    }

    return y;
}


size_t fj_size_next_power_of_two(size_t x)
{
    if (x >= ~(SIZE_MAX >> 1)) {
        return SIZE_MAX;
    }

    size_t y = 1;

    while (y <= x) {
        y *= 2;
    }

    return y;
}


size_t fj_size_prev_power_of_two(size_t x)
{
    if (x == 0) {
        return 1;
    }

    size_t y = 1;

    while (y < x - y) {
        y *= 2;
    }

    return y;
}


uint32_t fj_u32_prev_multiple(uint32_t x, uint32_t divisor)
{
    return x - x % divisor;
}


uint64_t fj_u64_prev_multiple(uint64_t x, uint64_t divisor)
{
    return x - x % divisor;
}
