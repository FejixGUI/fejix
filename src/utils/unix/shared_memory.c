#undef _GNU_SOURCE
#define _GNU_SOURCE

#include <src/unix/shared_memory.h>

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

static enum fj_status open_shm_file(int32_t *out_fd)
{
    *out_fd = memfd_create("fejix-shared-memory-file", MFD_CLOEXEC);

    if (*out_fd == -1) {
        return FJ_STATUS_IO_FAILED;
    }

    return FJ_STATUS_OK;
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
            return FJ_STATUS_IO_FAILED;
        }

        shm_unlink(temp_file_name);
        return FJ_STATUS_OK;
    }

    return FJ_STATUS_IO_FAILED;
}

#endif


static enum fj_status shm_map(struct fj_unix_shared_buffer *buffer)
{
    if (ftruncate(buffer->file, (off_t) buffer->size) == -1) {
        return FJ_STATUS_IO_FAILED;
    }

    buffer->data = mmap(NULL, buffer->size, PROT_READ | PROT_WRITE, MAP_SHARED, buffer->file, 0);

    if (buffer->data == MAP_FAILED) {
        return FJ_STATUS_IO_FAILED;
    }

    return FJ_STATUS_OK;
}


static enum fj_status shm_unmap(struct fj_unix_shared_buffer *buffer)
{
    if (munmap(buffer->data, buffer->size) == -1) {
        return FJ_STATUS_IO_FAILED;
    }

    return FJ_STATUS_OK;
}


enum fj_status fj_unix_shared_alloc(struct fj_unix_shared_buffer *out_buffer, size_t size)
{
    enum fj_status e;

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
        return fj_result;
    }

    return FJ_STATUS_OK;
}


enum fj_status fj_unix_shared_unref(struct fj_unix_shared_buffer *buffer)
{
    if (close(buffer->file) == -1) {
        return FJ_STATUS_IO_FAILED;
    }

    buffer->file = -1;
    buffer->data = NULL;
    buffer->size = 0;

    return FJ_STATUS_OK;
}


enum fj_status fj_unix_shared_free(struct fj_unix_shared_buffer *buffer)
{
    enum fj_status e;

    e = shm_unmap(buffer);

    if (e)
        return e;

    return fj_unix_shared_unref(buffer);
}


enum fj_status fj_unix_shared_realloc(struct fj_unix_shared_buffer *buffer, size_t size)
{
    enum fj_status err;

    if (size <= buffer->size) {
        return FJ_STATUS_OK;
    }

    buffer->size = fj_size_max(fj_size_higher_pow2(size), buffer->size);

    if (err = shm_map(buffer)) {
        return fj_result;
    }

    return FJ_STATUS_OK;
}
