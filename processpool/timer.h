//
// Created by magox on 7/3/20.
//

#ifndef PROCESSPOOL_TIMER_H
#define PROCESSPOOL_TIMER_H
#include <string.h>
#include <stdlib.h>

enum
{
    RET_OK,
    RET_FAIL
};

struct TimerInterface;
typedef struct TimerInterface TimerInterface;


struct TimerInterface
{
    int (*add)(int timeout,int fd,void(*handler)(int, void* ),void * args);
    int (*del)(void *timer);
};

static inline int add(TimerInterface *thiz, int timeout,int fd,void(*handler)(int, void* ),void * args)
{
    if(thiz->add != NULL)
    {
        return thiz->add(timeout, fd, handler,args);
    }
    return RET_OK;
}

static inline int del (TimerInterface *thiz, void *timer)
{
    if(thiz->del != NULL)
    {
        return thiz->del(timer);
    }
    return RET_OK;
}



#endif //PROCESSPOOL_TIMER_H
