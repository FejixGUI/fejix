#include <fejix/implementation.h>
#include <fejix/interface/client.h>

#include <fejix/ext/helper.h>

#include <fejix/core/utils.h>
#include <fejix/core/error.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>




struct fj_implementation const * impl = NULL;
fj_client_t * client;


fj_err_t callback(
    struct fj_client_callback const * callback,
    struct fj_message const * message
)
{
    puts("wow");
    impl->client->set_timeout(client, NAN);

    return FJ_OK;
}



int main(void)
{
    FJ_INIT_TRY

    struct fj_implementation const *const */*[]?*/ implementations;
    uint32_t implementation_count;

    fj_get_builtin_implementations(&implementations, &implementation_count);

    uint8_t const */*[]?*/ implementation_hint;

    implementation_hint = fj_ext_get_implementation_hint(implementations, implementation_count);

    if (implementation_hint == NULL) {
        puts("no implementation hint");
        return 1;
    }

    impl = fj_ext_find_implementation(implementations, implementation_count, implementation_hint);

    if (impl == NULL) {
        puts("cannot find implementation");
        return 2;
    }

    printf("implementation: %s\n", fj_get_implementation_name(impl->implementation_id));


    struct fj_client_callback const client_callback = {
        .call = callback
    };

    struct fj_client_info const client_info = {
        .name = FJ_UTF8("dev.fejix.test.dev"),
        .callback = &client_callback
    };

    fj_try impl->client->create(&client, &client_info);
    fj_else {
        printf("%s\n", fj_get_error_description(fj_result));
        return -1;
    }

    fj_try impl->client->run(client, FJ_CLIENT_RUN_TYPE_MAIN, NULL);
    fj_else {
        printf("%s\n", fj_get_error_description(fj_result));
        return -1;
    }

    fj_try impl->client->destroy(client);
    fj_else {
        printf("%s\n", fj_get_error_description(fj_result));
        return -1;
    }

    return 0;
}
