#ifndef FEJIX_COMPOSITING_MANAGER_H_
#define FEJIX_COMPOSITING_MANAGER_H_


#include <fejix/base.h>
#include <fejix/interface/server.h>
#include <fejix/interface/rendering_manager.h>
#include <fejix/interface/display_manager.h>


FJ_DEFINE_UNIQUE_TYPE(fj_compositing_manager_t)
FJ_DEFINE_UNIQUE_TYPE(fj_compositing_context_t)


enum fj_compositing_manager_message_id {
    FJ_COMPOSITING_CONTEXT_CREATED,
    FJ_COMPOSITING_CONTEXT_DESTROYED,
};


struct fj_compositing_context_info {
    fj_compositing_context_t *fjOPTION old_context;
    fj_rendering_context_t * source;
    fj_compositing_context_t *fjOPTION destination;
};

struct fj_compositing_context_desc {
    fj_bool32_t allows_subcompositing;
};

struct fj_compositing_manager {
    fj_err_t (* create)(
        fj_server_t * server,
        fj_compositing_manager_t *fjOPTION fjOUT * manager
    );

    fj_err_t (* destroy)(
        fj_compositing_manager_t * manager
    );

    fj_err_t (* create_context)(
        fj_compositing_manager_t * manager,
        struct fj_compositing_context_info const * context_info,
        fj_compositing_context_t *fjOPTION fjOUT * context
    );

    fj_err_t (* destroy_context)(
        fj_compositing_manager_t * manager,
        fj_compositing_context_t * context
    );

    void (* get_context_desc)(
        fj_compositing_manager_t * manager,
        fj_compositing_context_t * context,
        struct fj_compositing_context_desc fjOUT * context_desc
    );

    fj_err_t (* set_context_attachments)(
        fj_compositing_manager_t * manager,
        fj_compositing_context_t * context,
        void const * attachments // TODO
    );
};


#endif
