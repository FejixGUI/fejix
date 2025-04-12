#ifndef _GNU_SOURCE
#    define _GNU_SOURCE
#endif

#include <src/unixbase/shm.h>

#include <fejix/utils/math.h>
#include <fejix/utils/memory.h>

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

static fj_err open_shm_file(int32_t *out_fd)
{
    *out_fd = memfd_create("fejix-unixbase_shm-file", MFD_CLOEXEC);

    if (*out_fd == -1) {
        return FJ_ERR_IO_FAILED;
    }

    return FJ_OK;
}

#else

static uint32_t rand32(void)
{
    int fd = open("/dev/urandom", O_RDONLY);

    uint32_t buf;
    read(fd, (void *) &buf, sizeof(buf));

    close(fd);

    return buf;
}


static fj_err open_shm_file(int32_t *out_fd)
{
    for (uint32_t i = 0; i < 16; i++) {
        char temp_file_name[32];
        snprintf(temp_file_name, 32, "%s%08x", "/fejix-unixbase_shm-", rand32());

        *out_fd = shm_open(temp_file_name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);

        if (*out_fd == -1 && errno == EEXIST) {
            continue;
        }

        if (*out_fd == -1) {
            return FJ_ERR_IO_FAILED;
        }

        shm_unlink(temp_file_name);
        return FJ_OK;
    }

    return FJ_ERR_IO_FAILED;
}

#endif  // FJ_UNIXSHM_USE_SHM


static fj_err shm_map(struct fj_unixbase_shm *shm)
{
    if (ftruncate(shm->file, (off_t) shm->size) == -1) {
        return FJ_ERR_IO_FAILED;
    }

    shm->data = mmap(NULL, shm->size, PROT_READ | PROT_WRITE, MAP_SHARED, shm->file, 0);

    if (shm->data == MAP_FAILED) {
        return FJ_ERR_IO_FAILED;
    }

    return FJ_OK;
}


static fj_err shm_unmap(struct fj_unixbase_shm *shm)
{
    if (munmap(shm->data, shm->size) == -1) {
        return FJ_ERR_IO_FAILED;
    }

    return FJ_OK;
}


fj_err fj_unixbase_shm_alloc(struct fj_unixbase_shm *out_shm, size_t size)
{
    out_shm->size = fj_size_higher_pow2(size);

    FJ_TRY (open_shm_file(&out_shm->file)) {
        return fj_result;
    }

    FJ_TRY (shm_map(out_shm)) {
        close(out_shm->file);
        out_shm->file = -1;
        out_shm->size = 0;
        out_shm->data = NULL;
        return fj_result;
    }

    return FJ_OK;
}


fj_err fj_unixbase_shm_unref(struct fj_unixbase_shm *shm)
{
    if (close(shm->file) == -1) {
        return FJ_ERR_IO_FAILED;
    }

    shm->file = -1;
    shm->data = NULL;
    shm->size = 0;

    return FJ_OK;
}


fj_err fj_unixbase_shm_free(struct fj_unixbase_shm *shm)
{
    FJ_TRY (shm_unmap(shm)) {
        return fj_result;
    }

    return fj_unixbase_shm_unref(shm);
}


fj_err fj_unixbase_shm_realloc(struct fj_unixbase_shm *shm, size_t size)
{
    if (size <= shm->size) {
        return FJ_OK;
    }

    shm->size = fj_size_max(fj_size_higher_pow2(size), shm->size);

    FJ_TRY (shm_map(shm)) {
        return fj_result;
    }

    return FJ_OK;
}
