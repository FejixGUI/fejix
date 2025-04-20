/**
    \file
*/

#ifndef FEJIX_CONNECTION_SERIALIZE_H_INCLUDED
#define FEJIX_CONNECTION_SERIALIZE_H_INCLUDED


#include <fejix/modules/connection.h>


FJ_METHOD_WITH_FALLBACK(fj_connection_serialize, void *, NULL, struct fj_connection *conn)


FJ_METHOD_LIST_BEGIN(fj_connection_serialize)
FJ_METHOD_LIST_ITEM(fj_connection_serialize)
FJ_METHOD_LIST_END()


#endif
