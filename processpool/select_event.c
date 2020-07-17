//
// Created by magox on 7/10/20.
//
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "select_event.h"

select_event * create_select_event(map_event_item_t * events){
    select_event *e;
    e = (select_event *)malloc( sizeof(select_event));
    events = (map_event_item_t *)malloc( sizeof(map_event_item_t));
    memset(e,0,sizeof(select_event ));
    memset(events,0,sizeof(map_event_item_t ));
    e->epoll_fd = epoll_create( 5 );
    assert( e->epoll_fd != -1 );
    hash_map_init(events);
    e->events = events;
    return e;
}

static event_item * new_event_item(select_event * e,int fd,int flag,void * args,void(*handler)(int fd,void * args) ){
    event_item * eventItem;
    struct epoll_event event;
    eventItem = (event_item *)malloc( sizeof(event_item));
    memset(eventItem,0,sizeof(event_item ));
    event.data.fd = fd;
    event.events = flag;
    epoll_ctl( e->epoll_fd, EPOLL_CTL_ADD, fd, &event );
    setnonblocking( fd );
    eventItem->event = &event;
    eventItem->fd = fd;
    eventItem->flag = flag;
    eventItem->args = args;
    eventItem->handle = handler;
    return eventItem;
}


void select_event_add(select_event * e,int fd,int flag,void(*handler)(int fd,void * args),void *args){
    event_item *eventItem;
    eventItem = new_event_item(e,fd,flag,args,handler);
    char * key = hash_map_get_key(int,2,fd,flag);
    hash_map_set(e->events,key,eventItem);
    hash_map_free_key(key);
}

void select_event_del(select_event * e,int fd,int flag){
    char * key = hash_map_get_key(int,2,fd,flag);
    event_item * eventItem = map_get(e->events,key);
    if(eventItem != NULL){
        epoll_ctl( e->epoll_fd, EPOLL_CTL_DEL, eventItem->fd, flag );
        hash_map_remove(e->events,key);
    }
    hash_map_free_key(key);
}

void select_loop(select_event * e){
    struct epoll_event events[10000];
    while (1){
        int number  = epoll_wait( e->epoll_fd, events, 10000, -1 );
        if( ( number <0 ) && ( errno !=EINTR ) )
        {
            printf("epoll failure\n");
            break;
        }
        for( int i = 0; i < number; i++ )
        {
            int fd = events[i].data.fd;
            int flag = events[i].events;
            char * key = hash_map_get_key(int,2,fd,flag);
            event_item ** eventItem = hash_map_get(e->events,key);
            hash_map_free_key(key);
            if( (eventItem!=NULL) && ( (*eventItem)->flag & flag ))
            {
                (*eventItem)->handle(fd,(*eventItem)->args);
            }
        }
    }
}