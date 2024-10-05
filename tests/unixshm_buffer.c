#include <src/unixshm_buffer/unixshm_buffer.h>

#include <assert.h>


int main(void)
{
    struct fj_unixshm buffer;

    assert(fj_unixshm_buffer_create(&buffer, (size_t) 640 * 480 * 4) == FJ_OK);

    assert(fj_unixshm_buffer_resize(&buffer, (size_t) 1024 * 768 * 4) == FJ_OK);

    size_t size1 = buffer.size;
    assert(fj_unixshm_buffer_resize(&buffer, (size_t) 500 * 500 * 4) == FJ_OK);
    size_t size2 = buffer.size;
    assert(!(size2 < size1));

    struct fj_unixshm buffer2;
    assert(fj_unixshm_buffer_create(&buffer2, (size_t) 640 * 480 * 4) == FJ_OK);
    assert(fj_unixshm_buffer_destroy(&buffer2) == FJ_OK);

    assert(fj_unixshm_buffer_destroy(&buffer) == FJ_OK);

    return 0;
}
