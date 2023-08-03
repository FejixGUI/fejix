#include <fejix/fejix.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


void my_hello_world(void) {
    puts("Hello world!");
}

#define MY_INTERFACE_ID 123

struct FJ_INTERFACE(my_interface) {
    void FJ_METHOD(hello_world)(void);
};

FJ_IMPL_BEGIN(my_interface, my_instance)
    FJ_IMPL(hello_world, my_hello_world)
FJ_IMPL_END()

fj_err_t my_module_init(struct fj_sys * sys)
{
    return fj_sys_set_interface(sys, MY_INTERFACE_ID, &my_instance);
}


int main() {

    struct fj_sys * sys = fj_sys_new();

    assert(sys != NULL);

    assert(my_module_init(sys) == FJ_OK);

    struct my_interface * interface;
    interface = fj_sys_get_interface(sys, MY_INTERFACE_ID);
    
    assert(interface == &my_instance);

    interface->hello_world();

    assert(fj_sys_set_interface(sys, MY_INTERFACE_ID, NULL) == FJ_OK);

    assert(fj_sys_get_interface(sys, MY_INTERFACE_ID) == NULL);

    fj_sys_del(sys);

    return 0;
}
