#include <fejix/client.h>

#include <stdio.h>
#include <stdlib.h>


fj_err_t client_init(struct fj_client * client)
{
    (void) client;
    return FJ_OK;
}

fj_err_t client_deinit(struct fj_client * client, fj_err_t err)
{
    (void) client;
    return err;
}

fj_err_t client_run(struct fj_client * client)
{
    (void) client;
    return FJ_OK;
}

FJ_REQUIRE_VERSION(fj_client_listener, v_0_2)
struct fj_client_listener listener = {
    .init = client_init,
    .deinit = client_deinit,
    .run = client_run,
};

int main(void) {
    fj_err_t err = FJ_OK;

    fj_utf8string_t selected_platform = fj_select_platform();
    uint32_t platform_count;
    const fj_utf8string_t * platform_names;
    fj_client_get_platforms(&platform_count, &platform_names);

    printf("Running platform \"%s\"...\n", selected_platform);
    printf("Available platforms: ");
    for (uint32_t i=0; i<platform_count; i++) {
        printf("%s, ", platform_names[i]);
    }
    printf(".\n");

    struct fj_client * client = fj_client_new("com.example.test");

    if (client == NULL) {
        return 1;
    }

    *fj_client_get_listener(client) = &listener;

    err = fj_client_run(client);
    if (err != FJ_OK) {
        puts(err);
        return 2;
    }

    fj_client_del(client);

    printf("Runned successfully\n");

    return 0;
}
