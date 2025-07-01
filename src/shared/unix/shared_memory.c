#undef _GNU_SOURCE
#define _GNU_SOURCE

#include "shared_memory.h"

#include <src/shared/utils/logging.h>
#include <src/shared/utils/math.h>
#include <src/shared/utils/memory.h>

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

static enum fj_status open_shm_file(int32_t *out_fd)
{
    *out_fd = memfd_create("fejix-shared-memory-file", MFD_CLOEXEC);

    if (*out_fd == -1) {
        FJ_ERROR("memfd_create failed");
        return FJ_IO_FAILED;
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


static enum fj_status open_shm_file(int32_t *out_fd)
{
    for (uint32_t i = 0; i < 16; i++) {
        char temp_file_name[32];
        snprintf(temp_file_name, 32, "%s%08x", "/fejix-shared-memory-", rand32());

        *out_fd = shm_open(temp_file_name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);

        if (*out_fd == -1 && errno == EEXIST) {
            continue;
        }

        if (*out_fd == -1) {
            FJ_ERROR("shm_open failed");
            return FJ_IO_FAILED;
        }

        shm_unlink(temp_file_name);
        return FJ_OK;
    }

    return FJ_IO_FAILED;
}

#endif


static enum fj_status shm_map(struct fj_unix_shared_buffer *buffer)
{
    if (ftruncate(buffer->file, (off_t) buffer->size) == -1) {
        return FJ_IO_FAILED;
    }

    buffer->data = mmap(NULL, buffer->size, PROT_READ | PROT_WRITE, MAP_SHARED, buffer->file, 0);

    if (buffer->data == MAP_FAILED) {
        FJ_ERROR("mmap failed");
        return FJ_IO_FAILED;
    }

    return FJ_OK;
}


static enum fj_status shm_unmap(struct fj_unix_shared_buffer *buffer)
{
    if (munmap(buffer->data, buffer->size) == -1) {
        FJ_ERROR("nunmap failed");
        return FJ_IO_FAILED;
    }

    return FJ_OK;
}


enum fj_status fj_unix_shared_alloc(struct fj_unix_shared_buffer *out_buffer, size_t size)
{
    enum fj_status s;

    out_buffer->size = fj_size_higher_pow2(size);

    s = open_shm_file(&out_buffer->file);
    if (s)
        return s;

    s = shm_map(out_buffer);

    if (s) {
        close(out_buffer->file);
        out_buffer->file = -1;
        out_buffer->size = 0;
        out_buffer->data = NULL;
        return s;
    }

    return FJ_OK;
}


enum fj_status fj_unix_shared_unref(struct fj_unix_shared_buffer *buffer)
{
    if (close(buffer->file) == -1) {
        FJ_ERROR("close(2) failed");
        return FJ_IO_FAILED;
    }

    buffer->file = -1;
    buffer->data = NULL;
    buffer->size = 0;

    return FJ_OK;
}


enum fj_status fj_unix_shared_free(struct fj_unix_shared_buffer *buffer)
{
    enum fj_status s;

    s = shm_unmap(buffer);

    if (s)
        return s;

    return fj_unix_shared_unref(buffer);
}


enum fj_status fj_unix_shared_realloc(struct fj_unix_shared_buffer *buffer, size_t size)
{
    enum fj_status s;

    if (size <= buffer->size) {
        return FJ_OK;
    }

    buffer->size = fj_size_max(fj_size_higher_pow2(size), buffer->size);

    s = shm_map(buffer);
    if (s)
        return s;

    return FJ_OK;
}
