#ifndef FEJIX_INTERNAL_WAYLAND_BUS_H_
#define FEJIX_INTERNAL_WAYLAND_BUS_H_


#include <fejix/bus.h>

#include <fejix/sockets/scheduler.h>

#include <src/shared/fdpoll/fdpoll.h>


struct fj_wayland_bus_context {
    struct fj_bus_context_base FJ_INHERIT base;

    struct fj_scheduler_schedule schedule;
    struct fj_fdpoll poller;
};


#endif
