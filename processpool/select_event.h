//
// Created by magox on 6/30/20.
//

#ifndef PROCESSPOOL_SELECT_EVENT_H
#define PROCESSPOOL_SELECT_EVENT_H

#include "hash_map.h"


typedef struct  {
    int fd;
    int flag;
    void * args;
    struct epoll_event *event;
    void (*handle)(int,void*);
    void (*write)(int,void*);
    void (*read)(int,void*);
} event_item;

typedef hash_map_t(event_item * ) map_event_item_t;

typedef struct {
    int epoll_fd;
    map_event_item_t * events;
} select_event;


static int setnonblocking( int fd )
{
    int old_option = fcntl( fd, F_GETFL );
    int new_option  = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}

select_event * create_select_event(map_event_item_t * events);
static event_item * new_event_item(select_event * e,int fd,int flag,void * args,void(*handler)(int fd,void * args) );
void select_event_add(select_event * e,int fd,int flag,void(*handler)(int fd,void * args),void *args);
void select_event_del(select_event * e,int fd,int flag);
void select_loop(select_event * e);

#endif //PROCESSPOOL_SELECT_EVENT_H
