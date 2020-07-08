//
// Created by magox on 7/2/20.
//

#ifndef PROCESSPOOL_TIMER_WHEEL_H
#define PROCESSPOOL_TIMER_WHEEL_H
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include "Interface/timer.h"


#define free_wheel(T)  \
    for(int i=0;i<WHEEL_SLOTS_NUM;i++){\
        if(timerWheel->slots[i]!=NULL){\
            free(timerWheel);\
            timerWheel->slots[i]=NULL;\
        }\
    }\
    free(T)\

enum
{
    WHEEL_STATUS_SETTING=0,
    WHEEL_STATUS_RUNNING,
    WHEEL_STATUS_STOP,
    WHEEL_STATUS_DESTRORING
};

typedef struct  timer_item timer_item ;


struct timer_item  {
    int fd;
    int rotation;
    int time_slot;
    timer_item * next;
    timer_item * prev;
    void * args;
    void (*handler)(int,void *)
};

#define WHEEL_SLOTS_NUM 10

typedef struct  {
    struct TimerInterface timerInterface;
    timer_item * slots[WHEEL_SLOTS_NUM];
    int n;
    int si;
    int status;
    int cur_slot;
    int counts;
} timer_wheel;

static _Bool is_null_timer(timer_item * timerItem);
static  timer_item * new_timer_item( timer_wheel *timerWheel,int ts,int fd,int rotation,void(*handler)(int,void *),void * args);
static inline int wheel_timer_add( timer_wheel * timerWheel,int timeout,int fd,void(*handler)(int, void* ),void * args);
static inline int wheel_timer_del(timer_wheel * timerWheel,timer_item * timerItem);
void wheel_start(timer_wheel * timerWheel);
void wheel_stop(timer_wheel * timerWheel);
void wheel_tick(timer_wheel * timerWheel);
static inline int wheel_destory(timer_wheel * timerWheel);
static inline timer_wheel * _get_thiz(TimerInterface *thiz);
static inline int _timer_wheel_add(TimerInterface *thiz,int timeout,int fd,void(*handler)(int, void* ),void * args);
static inline int _timer_wheel_del(TimerInterface *thiz,void *timer);
static inline timer_wheel * create_timer_wheel (int si,int n);
static inline TimerInterface * get_thiz_by_timer_wheel(timer_wheel* timerWheel);

static _Bool is_null_timer(timer_item * timerItem){
    if(timerItem==NULL){
        return true;
    }
    return false;
}

static  timer_item * new_timer_item( timer_wheel *timerWheel,int ts,int fd,int rotation,void(*handler)(int,void *),void * args){
    timer_item * timerItem;
    timerItem = (timer_item *)malloc( sizeof(struct timer_item ) );
    timerItem->args = args;
    timerItem->rotation = rotation;
    timerItem->time_slot = ts;
    timerItem->handler = handler;
    timerItem->fd = fd;
    timerItem->next = NULL;
    timerItem->prev = NULL;
    if( is_null_timer(timerWheel->slots[ts])){
        timerWheel->slots[ts] = timerItem ;
    }else{
        timerItem->next = timerWheel->slots[ts];
        timerWheel->slots[ts]->prev = timerItem;
        timerWheel->slots[ts] = timerItem;
    }
    return timerItem;
}

static inline int wheel_timer_add( timer_wheel * timerWheel,int timeout,int fd,void(*handler)(int, void* ),void * args){
    if(timerWheel->status==WHEEL_STATUS_DESTRORING){
        return RET_FAIL;
    }
    if(timeout <0){
        return NULL;
    }
    int ticks = 0;
    if(timeout<(timerWheel->si)){
        ticks = 1;
    }else{
        ticks = timeout/(timerWheel->si);
    }
    int rotation = ticks/(timerWheel->n);
    int ts = (timerWheel->cur_slot +(ticks % timerWheel->n)) % timerWheel->n;
    timer_item * timerItem =new_timer_item(timerWheel,ts,fd,rotation,handler,args) ;
    timerWheel->counts++;
    return RET_OK;
}

static inline int wheel_timer_del(timer_wheel * timerWheel,timer_item * timerItem)
{
    int ts = timerItem->time_slot;
    if(timerItem==timerWheel->slots[ts]){
        timerWheel->slots[ts] = timerWheel->slots[ts]->next;
        if(!is_null_timer(timerWheel->slots[ts])){
            timerWheel->slots[ts]->prev = NULL;
        }
        free(timerItem);
    }else{
        timerItem->prev->next = timerItem->next;
        if(!is_null_timer(timerItem->next)){
            timerItem->next->prev = timerItem->prev;
        }
        free(timerItem);
    }
    timerWheel->counts--;
    return RET_OK;
}

void wheel_start(timer_wheel * timerWheel){
    timerWheel->status=WHEEL_STATUS_RUNNING;
}

void wheel_stop(timer_wheel * timerWheel){
    timerWheel->status=WHEEL_STATUS_STOP;
}

void wheel_tick(timer_wheel * timerWheel){
    timer_item * tmp;
    tmp = timerWheel->slots[timerWheel->cur_slot];
    while (!is_null_timer(tmp) && timerWheel->status==WHEEL_STATUS_RUNNING){
        if(tmp->rotation>0){
            tmp->rotation--;
            tmp = tmp->next;
        }else{
            tmp->handler(tmp->fd,tmp->args);
            if(tmp==timerWheel->slots[timerWheel->cur_slot]){
                timerWheel->slots[timerWheel->cur_slot] = tmp->next;
                if(!is_null_timer(timerWheel->slots[timerWheel->cur_slot])){
                    timerWheel->slots[timerWheel->cur_slot]->prev = NULL;
                }
                free(tmp);
                tmp = timerWheel->slots[timerWheel->cur_slot];
            }else{
                tmp->prev->next = tmp->next;
                if(!is_null_timer(tmp->next)){
                    tmp->next->prev = tmp->prev;
                }
                timer_item *tmp2 = tmp->next;
                free(tmp);
                tmp = tmp2;
            }
        }
    }

    if(timerWheel->status==WHEEL_STATUS_DESTRORING && timerWheel->counts==0){
        free_wheel(timerWheel);
    }
    timerWheel->cur_slot = ++timerWheel->cur_slot % timerWheel->n;
}

static inline int wheel_destory(timer_wheel * timerWheel){
    timerWheel->status=WHEEL_STATUS_DESTRORING;
    if(timerWheel->counts==0){
        free_wheel(timerWheel);
    }
}


static inline int _timer_wheel_add(TimerInterface *thiz,int timeout,int fd,void(*handler)(int, void* ),void * args)
{
    timer_wheel * timerWheel = get_thiz_parent(timer_wheel,timerInterface,thiz);
    return wheel_timer_add( timerWheel,timeout,fd,handler,args);
}

static inline int _timer_wheel_del(TimerInterface *thiz,void *timer)
{
    timer_wheel * timerWheel = get_thiz_parent(timer_wheel,timerInterface,thiz);
    return wheel_timer_del(timerWheel,(timer_item *)timer);
}

static inline timer_wheel * create_timer_wheel (int si,int n){
    timer_wheel * timerWheel;
    timerWheel = (timer_wheel *)malloc( sizeof( timer_wheel));
    memset(timerWheel,"\0",sizeof(timer_wheel));
    for(int i=0;i<n;i++){
        timerWheel->slots[i] = NULL;
    }
    timerWheel->n = n;
    timerWheel->si = si;
    timerWheel->cur_slot = 0;
    timerWheel->counts = 0;
    timerWheel->timerInterface.add = _timer_wheel_add;
    timerWheel->timerInterface.del = _timer_wheel_del;
    timerWheel->status=WHEEL_STATUS_SETTING;
    return timerWheel;
}

#endif //PROCESSPOOL_TIMER_WHEEL_H
