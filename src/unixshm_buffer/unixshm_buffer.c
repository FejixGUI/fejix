#include <src/unixshm_buffer/unixshm_buffer.h>

#include <fejix/core/alloc.h>
#include <fejix/core/any.h>
#include <fejix/core/utils.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>


#ifdef __linux__

static
fj_err_t open_shm_file(int32_t /*out*/ * fd)
{
    *fd = memfd_create("fejix-unixshm-buffer-file", MFD_CLOEXEC);

    if (*fd == -1) {
        return FJ_ERR_TEMP_FILE_CREATION_FAILED;
    }

    return FJ_OK;
}


#else // !defined(__linux__)

static
uint32_t rand32(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    uint64_t value1 = ts.tv_nsec;
    uint64_t value2 = ts.tv_sec;
    uint32_t hash1 = fj_any_hash32(FJ_U64(value1), FJ_ANY_U64);
    uint32_t hash2 = fj_any_hash32(FJ_U64(value2), FJ_ANY_U64);
    return hash1 ^ hash2;
}


/** The returned string should be manually freed. */
static
fj_err_t create_shm_file_name(char const */*? out*/ * name)
{
    FJ_INIT_TRY

    char const * base_name = "/fejix-unixshm-buffer-file-";

    // + strlen("ffffffff") + '\0'
    size_t name_length = strlen(base_name) + 8 + 1;

    FJ_TRY(fj_alloc_zeroed((void *) name, sizeof(char) * name_length)) {
        return FJ_RESULT;
    }

    snprintf((char *) *name, name_length, "%s%08x", base_name, rand32());

    return FJ_OK;
}


static
fj_err_t open_shm_file(int32_t /*out*/ * fd)
{
    FJ_INIT_TRY

    for (uint32_t i=0; i<16; i++) {
        char const * file_name;
        FJ_TRY(create_shm_file_name(&file_name)) {
            return FJ_RESULT;
        }

        *fd = shm_open(file_name, O_RDWR|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR);

        if (*fd == -1 && errno == EEXIST) {
            FJ_FREE(&file_name);
            continue;
        }

        if (*fd == -1) {
            FJ_FREE(&file_name);
            return FJ_ERR_TEMP_FILE_CREATION_FAILED;
        }

        shm_unlink(file_name);
        FJ_FREE(&file_name);
        return FJ_OK;
    }

    return FJ_ERR_TEMP_FILE_CREATION_FAILED;
}

#endif // !defined(__linux__)


static
fj_err_t shm_map(struct fj_unixshm_buffer * shm)
{
    if (ftruncate(shm->file, (off_t) shm->size) == -1) {
        return FJ_ERR_TEMP_FILE_CREATION_FAILED;
    }

    shm->data = mmap(NULL, shm->size, PROT_READ|PROT_WRITE, MAP_SHARED, shm->file, 0);

    if (shm->data == MAP_FAILED) {
        return FJ_ERR_SHARED_MEMORY_ALLOCATION_FAILED;
    }

    return FJ_OK;
}


static
fj_err_t shm_unmap(struct fj_unixshm_buffer * shm)
{
    if (munmap(shm->data, shm->size) == -1) {
        return FJ_ERR_SHARED_MEMORY_ALLOCATION_FAILED;
    }

    return FJ_OK;
}


fj_err_t fj_unixshm_buffer_create(struct fj_unixshm_buffer /*out*/ * buffer, size_t size)
{
    FJ_INIT_TRY

    buffer->size = fj_size_next_power_of_two(size);

    FJ_TRY(open_shm_file(&buffer->file)) {
        return FJ_RESULT;
    }

    FJ_TRY(shm_map(buffer)) {
        close(buffer->file);
        buffer->file = -1;
        buffer->size = 0;
        buffer->data = NULL;
        return FJ_RESULT;
    }

    return FJ_OK;
}


fj_err_t fj_unixshm_buffer_destroy(struct fj_unixshm_buffer * buffer)
{
    FJ_INIT_TRY

    FJ_TRY(shm_unmap(buffer)) {
        return FJ_RESULT;
    }

    if(close(buffer->file) == -1) {
        return FJ_ERR_SHARED_MEMORY_ALLOCATION_FAILED;
    }

    buffer->file = -1;
    buffer->data = NULL;
    buffer->size = 0;

    return FJ_OK;
}


fj_err_t fj_unixshm_buffer_resize(struct fj_unixshm_buffer * buffer, size_t size)
{
    FJ_INIT_TRY

    if (size <= buffer->size) {
        return FJ_OK;
    }

    buffer->size = fj_size_next_power_of_two(size);

    FJ_TRY(shm_map(buffer)) {
        return FJ_RESULT;
    }

    return FJ_OK;
}


void * fj_unixshm_buffer_get_data(struct fj_unixshm_buffer * buffer)
{
    return buffer->data;
}
