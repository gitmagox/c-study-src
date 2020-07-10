//
// Created by magox on 7/7/20.
//

#ifndef PROCESSPOOL_PROTOCOL_H
#define PROCESSPOOL_PROTOCOL_H

#include "base.h"

static inline int input(ProtocolInterface *thiz, char* buffer,void * connection)
{
    if(thiz->input != NULL)
    {
        return thiz->input(thiz,buffer, connection);
    }
    return RET_OK;
}

static inline int encode(ProtocolInterface *thiz, char* buffer,void * connection)
{
    if(thiz->encode != NULL)
    {
        return thiz->encode(thiz,buffer, connection);
    }
    return RET_OK;
}

static inline void * decode(ProtocolInterface *thiz, char* buffer,void * connection)
{
    if(thiz->decode != NULL)
    {
        return thiz->decode(thiz,buffer, connection);
    }
    return NULL;
}

#endif //PROCESSPOOL_PROTOCOL_H
