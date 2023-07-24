#ifndef FEJIX_IDLIST_H_
#define FEJIX_IDLIST_H_


#include <fejix/core/types.h>


struct fj_idlist;


/// Linear array of identifiers (`fj_id_t`).
typedef struct fj_idlist fj_idlist_t;


struct fj_idlist {
    fj_id_t * elements;
    uint32_t length;
    uint32_t capacity;
};


fj_idlist_t * fj_idlist_new(void);

void fj_idlist_del(fj_idlist_t * list);

fj_result_t fj_idlist_insert(fj_idlist_t * list, uint32_t index, fj_id_t elem);

fj_result_t fj_idlist_remove(fj_idlist_t * list, uint32_t index);

fj_result_t fj_idlist_push(fj_idlist_t * list, fj_id_t element);

fj_result_t fj_idlist_pop(fj_idlist_t * list);


#endif