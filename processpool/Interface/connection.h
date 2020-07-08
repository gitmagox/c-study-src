//
// Created by magox on 7/8/20.
//

#ifndef PROCESSPOOL_CONNECTION_H
#define PROCESSPOOL_CONNECTION_H

#include "base.h"

struct ConnectionInterface;
typedef struct ConnectionInterface ConnectionInterface;

struct ConnectionInterface
{
    int (*send)(ConnectionInterface  *thiz,char* buffer);
    int (*close)(ConnectionInterface *thiz);
};

static inline int send(ConnectionInterface *thiz, char* buffer)
{
    if(thiz->send != NULL)
    {
        return thiz->send(thiz,buffer);
    }
    return RET_OK;
}

static inline int close(ConnectionInterface *thiz)
{
    if(thiz->close != NULL)
    {
        return thiz->close(thiz);
    }
    return RET_OK;
}


#endif //PROCESSPOOL_CONNECTION_H
