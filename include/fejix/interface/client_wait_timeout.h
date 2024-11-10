#ifndef FEJIX_INTERFACE_CLIENT_WAIT_TIMEOUT_H_
#define FEJIX_INTERFACE_CLIENT_WAIT_TIMEOUT_H_


#include <fejix/interface/client.h>


struct fj_client_wait_timeout_funcs {
    /**
        Sets the preferred amount of time that the client should be allowed to wait for incoming
        messages.

        When the timeout expires, the client calls its idle callback.

        The timeout is always clamped to the platform's limits (normally 1ms..2^31ms).

        Wait timeouts are in practice very imprecise, actual wait times may experience:
        * occasional overruns (because of job scheduling)
        * rounding to the nearest system clock period (e.g. 1/64 of a second by default on Windows)
        * unpredictable rounding errors
        * unpredictable drifts for bigger timeouts
    */
    void (*set_timeout)(struct fj_client *client, fj_timeout_t timeout);

    void (*unset_timeout)(struct fj_client *client);
};


#endif
