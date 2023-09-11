#include <fejix/client.h>

#include <stdio.h>


int main() {
    fj_err_t err = FJ_OK;

    struct fj_client * client = fj_client_new("com.example.test");

    if (client == NULL) {
        return 1;
    }

    const struct fj_client_listener ** client_listener = fj_client_get_listener(client);
    *client_listener = (void *) 1;

    err = fj_client_run(client);
    if (err != FJ_OK) {
        puts(err);
        return 2;
    }

    fj_client_del(client);

    return 0;
}
