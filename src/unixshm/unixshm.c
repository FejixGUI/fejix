#include <src/unixshm/unixshm.h>

#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#ifdef __linux__
#    define FJ_UNIXSHM_USE_MEMFD
#else
#    define FJ_UNIXSHM_USE_SHM
#endif


#ifdef FJ_UNIXSHM_USE_MEMFD

static fj_err open_shm_file(int32_t /*out*/ *fd)
{
    *fd = memfd_create("fejix-unixshm-file", MFD_CLOEXEC);

    if (*fd == -1) {
        return FJ_ERR_TEMP_FILE_CREATION_FAILED;
    }

    return FJ_OK;
}

#endif  // FJ_UNIXSHM_USE_MEMFD


#ifdef FJ_UNIXSHM_USE_SHM

static uint32_t rand32(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    uint64_t value1 = ts.tv_nsec;
    uint64_t value2 = ts.tv_sec;
    uint32_t hash1 = fj_any_hash32(FJ_U64(value1), FJ_ANY_U64);
    uint32_t hash2 = fj_any_hash32(FJ_U64(value2), FJ_ANY_U64);
    return hash1 ^ hash2;
}


static fj_err open_shm_file(int32_t /*out*/ *fd)
{
    for (uint32_t i = 0; i < 16; i++) {
        char temp_file_name[32];
        snprintf(temp_file_name, 32, "%s%08x", "/fejix-unixshm-", rand32());

        *fd = shm_open(temp_file_name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);

        if (*fd == -1 && errno == EEXIST) {
            continue;
        }

        if (*fd == -1) {
            return FJ_ERR_TEMP_FILE_CREATION_FAILED;
        }

        shm_unlink(temp_file_name);
        return FJ_OK;
    }

    return FJ_ERR_TEMP_FILE_CREATION_FAILED;
}

#endif  // FJ_UNIXSHM_USE_SHM


static fj_err shm_map(struct fj_unixshm *shm)
{
    if (ftruncate(shm->file, (off_t) shm->size) == -1) {
        return FJ_ERR_SHARED_MEMORY_ALLOCATION_FAILED;
    }

    shm->data = mmap(NULL, shm->size, PROT_READ | PROT_WRITE, MAP_SHARED, shm->file, 0);

    if (shm->data == MAP_FAILED) {
        return FJ_ERR_SHARED_MEMORY_ALLOCATION_FAILED;
    }

    return FJ_OK;
}


static fj_err shm_unmap(struct fj_unixshm *shm)
{
    if (munmap(shm->data, shm->size) == -1) {
        return FJ_ERR_SHARED_MEMORY_ALLOCATION_FAILED;
    }

    return FJ_OK;
}


fj_err fj_unixshm_alloc(struct fj_unixshm /*out*/ *shm, size_t size)
{
    shm->size = fj_size_next_power_of_two(size);

    FJ_TRY (open_shm_file(&shm->file)) {
        return fj_result;
    }

    FJ_TRY (shm_map(shm)) {
        close(shm->file);
        shm->file = -1;
        shm->size = 0;
        shm->data = NULL;
        return fj_result;
    }

    return FJ_OK;
}


fj_err fj_unixshm_unref(struct fj_unixshm *shm)
{
    if (close(shm->file) == -1) {
        return FJ_ERR_SHARED_MEMORY_ALLOCATION_FAILED;
    }

    shm->file = -1;
    shm->data = NULL;
    shm->size = 0;

    return FJ_OK;
}


fj_err fj_unixshm_free(struct fj_unixshm *shm)
{
    FJ_TRY (shm_unmap(shm)) {
        return fj_result;
    }

    return fj_unixshm_unref(shm);
}


fj_err fj_unixshm_realloc(struct fj_unixshm *shm, size_t size)
{
    if (size <= shm->size) {
        return FJ_OK;
    }

    shm->size = fj_size_next_power_of_two(size);

    FJ_TRY (shm_map(shm)) {
        return fj_result;
    }

    return FJ_OK;
}
