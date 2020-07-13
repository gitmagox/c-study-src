//
// Created by magox on 7/8/20.
//

#ifndef PROCESSPOOL_CONNECTION_H
#define PROCESSPOOL_CONNECTION_H

#include "base.h"


static inline int ssend(ConnectionInterface *thiz, char* buffer)
{
    if(thiz->send != NULL)
    {
        return thiz->send(thiz,buffer);
    }
    return RET_OK;
}

static inline int cclose(ConnectionInterface *thiz,char* message)
{
    if(thiz->close != NULL)
    {
        return thiz->close(thiz,message);
    }
    return RET_OK;
}

static inline int destroy(ConnectionInterface *thiz)
{
    if(thiz->destroy != NULL)
    {
        return thiz->destroy(thiz);
    }
    return RET_OK;
}




#endif //PROCESSPOOL_CONNECTION_H
