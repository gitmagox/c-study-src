//
// Created by magox on 7/8/20.
//

#ifndef PROCESSPOOL_CONNECTION_H
#define PROCESSPOOL_CONNECTION_H

#include "base.h"


//static inline int send(ConnectionInterface *thiz, char* buffer)
//{
//    if(thiz->send != NULL)
//    {
//        return thiz->send(thiz,buffer);
//    }
//    return RET_OK;
//}
//
//static inline int close(ConnectionInterface *thiz)
//{
//    if(thiz->close != NULL)
//    {
//        return thiz->close(thiz);
//    }
//    return RET_OK;
//}

static inline int destroy(ConnectionInterface *thiz)
{
    if(thiz->destroy != NULL)
    {
        return thiz->destroy(thiz);
    }
    return RET_OK;
}




#endif //PROCESSPOOL_CONNECTION_H
