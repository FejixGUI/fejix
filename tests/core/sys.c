#include <fejix/fejix.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


void my_hello_world(void) {
    puts("Hello world!");
}

#define MY_INTERFACE_ID 123

FJ_DEFINE_INTERFACE(my_interface) {
    FJ_DEFINE_METHOD(hello_world, void, (void))
};

FJ_IMPLEMENT_INTERFACE(my_interface, my_instance) {
    FJ_IMPLEMENT_METHOD(hello_world, my_hello_world)
};

#define MY_MODULE_ID 321

fj_err_t my_module_init(struct fj_sys * sys)
{
    return fj_sys_set_interface(
        sys, MY_MODULE_ID, MY_INTERFACE_ID, &my_instance
    );
}


int main() {

    struct fj_sys * sys = fj_sys_new();

    assert(sys != NULL);

    assert(my_module_init(sys) == fj_ok);

    struct my_interface * interface;
    interface = fj_sys_find_interface(sys, MY_INTERFACE_ID);
    
    assert(interface == &my_instance);

    interface->hello_world();

    assert(fj_sys_set_interface(sys, MY_MODULE_ID, MY_INTERFACE_ID, NULL) == fj_ok);

    assert(fj_sys_find_interface(sys, MY_INTERFACE_ID) == NULL);

    fj_sys_del(sys);

    return 0;
}