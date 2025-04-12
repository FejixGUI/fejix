#include <src/unixbase/shm.h>

#include <assert.h>


int main(void)
{
    struct fj_unixbase_shm shm;

    assert(fj_unixbase_shm_alloc(&shm, (size_t) 640 * 480 * 4) == FJ_OK);

    assert(fj_unixbase_shm_realloc(&shm, (size_t) 1024 * 768 * 4) == FJ_OK);

    size_t size1 = shm.size;
    assert(fj_unixbase_shm_realloc(&shm, (size_t) 500 * 500 * 4) == FJ_OK);
    size_t size2 = shm.size;
    assert(!(size2 < size1));

    struct fj_unixbase_shm shm2;
    assert(fj_unixbase_shm_alloc(&shm2, (size_t) 640 * 480 * 4) == FJ_OK);
    assert(fj_unixbase_shm_free(&shm2) == FJ_OK);

    assert(fj_unixbase_shm_free(&shm) == FJ_OK);

    return 0;
}
