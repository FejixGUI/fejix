#include <src/unix/shared_memory.h>

#include <assert.h>


int main(void)
{
    struct fj_unix_shared_buffer buffer;

    assert(fj_unix_shared_alloc(&buffer, (size_t) 640 * 480 * 4) == FJ_OK);

    assert(fj_unix_shared_realloc(&buffer, (size_t) 1024 * 768 * 4) == FJ_OK);

    size_t size1 = buffer.size;
    assert(fj_unix_shared_realloc(&buffer, (size_t) 500 * 500 * 4) == FJ_OK);
    size_t size2 = buffer.size;
    assert(!(size2 < size1));

    struct fj_unix_shared_buffer buffer2;
    assert(fj_unix_shared_alloc(&buffer2, (size_t) 640 * 480 * 4) == FJ_OK);
    assert(fj_unix_shared_free(&buffer2) == FJ_OK);

    assert(fj_unix_shared_free(&buffer) == FJ_OK);

    return 0;
}
