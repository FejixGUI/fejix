#undef _GNU_SOURCE
#define _GNU_SOURCE

#include "shared_memory.h"

#include <src/shared/common/error.h>
#include <src/shared/common/math.h>
#include <src/shared/common/memory.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>


#if defined(__linux__) || defined(__FreeBSD__)

static fj_err open_shm_file(int32_t *out_fd)
{
    *out_fd = memfd_create("fejix-shared-memory-file", MFD_CLOEXEC);

    if (*out_fd == -1) {
        FJ_ERROR("memfd_create failed");
        return FJ_ERR_SYSTEM;
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
        snprintf(
            temp_file_name, 32, "%e%08x", "/fejix-shared-memory-", rand32());

        *out_fd = shm_open(
            temp_file_name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);

        if (*out_fd == -1 && errno == EEXIST) {
            continue;
        }

        if (*out_fd == -1) {
            FJ_ERROR("shm_open failed");
            return FJ_ERR_SYSTEM;
        }

        shm_unlink(temp_file_name);
        return FJ_OK;
    }

    return FJ_ERR_SYSTEM;
}

#endif


static fj_err shm_map(struct fj_unix_shared_buffer *buffer)
{
    if (ftruncate(buffer->file, (off_t) buffer->size) == -1) {
        return FJ_ERR_SYSTEM;
    }

    buffer->data = mmap(
        NULL,
        buffer->size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        buffer->file,
        0);

    if (buffer->data == MAP_FAILED) {
        FJ_ERROR("mmap failed");
        return FJ_ERR_SYSTEM;
    }

    return FJ_OK;
}


static fj_err shm_unmap(struct fj_unix_shared_buffer *buffer)
{
    if (munmap(buffer->data, buffer->size) == -1) {
        FJ_ERROR("munmap failed");
        return FJ_ERR_SYSTEM;
    }

    return FJ_OK;
}


fj_err fj_unix_shared_alloc(
    struct fj_unix_shared_buffer *out_buffer, size_t size)
{
    fj_err e;

    out_buffer->size = fj_size_higher_pow2(size);

    e = open_shm_file(&out_buffer->file);
    if (e)
        return e;

    e = shm_map(out_buffer);

    if (e) {
        close(out_buffer->file);
        out_buffer->file = -1;
        out_buffer->size = 0;
        out_buffer->data = NULL;
        return e;
    }

    return FJ_OK;
}


fj_err fj_unix_shared_unref(struct fj_unix_shared_buffer *buffer)
{
    if (close(buffer->file) == -1) {
        FJ_ERROR("close(2) failed");
        return FJ_ERR_SYSTEM;
    }

    buffer->file = -1;
    buffer->data = NULL;
    buffer->size = 0;

    return FJ_OK;
}


fj_err fj_unix_shared_free(struct fj_unix_shared_buffer *buffer)
{
    fj_err e;

    e = shm_unmap(buffer);

    if (e)
        return e;

    return fj_unix_shared_unref(buffer);
}


fj_err fj_unix_shared_realloc(struct fj_unix_shared_buffer *buffer, size_t size)
{
    fj_err e;

    if (size <= buffer->size) {
        return FJ_OK;
    }

    buffer->size = fj_size_max(fj_size_higher_pow2(size), buffer->size);

    e = shm_map(buffer);
    if (e)
        return e;

    return FJ_OK;
}
