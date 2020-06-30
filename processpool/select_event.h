//
// Created by magox on 6/30/20.
//

#ifndef PROCESSPOOL_SELECT_EVENT_H
#define PROCESSPOOL_SELECT_EVENT_H

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
#include "map.h"

typedef struct  {
    int fd;
    int flag;
    void * args;
    struct epoll_event *event;
    void (*handle)(int fd,void * args);
} event_item;

typedef struct {
    int epoll_fd;
    map_void_t * events;
} select_event;


static int setnonblocking( int fd )
{
    int old_option = fcntl( fd, F_GETFL );
    int new_option  = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}

select_event * create_select_event(){
    select_event *e;
    memset(e,"\0",sizeof(select_event ));
    e->epoll_fd = epoll_create( 5 );
    assert( e->epoll_fd != -1 );
    map_init(e->events);
    return e;
}

static event_item * new_event_item(select_event * e,int fd,int flag,void * args,void(*handler)(int fd,void * args) ){
    event_item * eventItem;
    struct epoll_event event;
    event.data.fd = fd;
    event.events = flag;
    epoll_ctl( e->epoll_fd, EPOLL_CTL_ADD, fd, &event );
    setnonblocking( fd );
    eventItem->event = &event;
    eventItem->flag = flag;
    eventItem->args = args;
    eventItem->handle = handler;
    return eventItem;
}

static char * add_string(char *a,char *b){
    char *name = (char *) malloc(strlen(a) + strlen(b));
    sprintf(name, "%s%s", a, b);
    return name;
}

static char * get_key(int fd,int flag){
    char * key;
    char fda[11],flaga[11];
    char *c = "k";
    sprintf(fda, " %d" , fd);
    sprintf(flaga, " %d" , flag);
    key = add_string(fda,add_string(c,flaga));
    return key;
}

void select_event_add(select_event * e,int fd,int flag,void(*handler)(int fd,void * args),void *args){
    event_item * eventItem = new_event_item(e,fd,flag,args,handler);
    char * key = get_key(fd,flag);
    map_set(e->events,key,eventItem);
}

void select_event_del(select_event * e,int fd,int flag){
    char * key = get_key(fd,flag);
    event_item * eventItem = map_get(e->events,key);
    if(eventItem != NULL){
        epoll_ctl( e->epoll_fd, EPOLL_CTL_DEL, eventItem->fd, 0 );
        close( fd );
        map_remove(e->events,key);
    }
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
            char * key = get_key(fd,flag);
            event_item * eventItem = map_get(e->events,key);
            if( (eventItem!=NULL) && ( eventItem->event->events & flag ))
            {
                eventItem->handle(fd,eventItem->args);
                select_event_del(e,fd,flag);
            }
        }
    }
}




#endif //PROCESSPOOL_SELECT_EVENT_H
