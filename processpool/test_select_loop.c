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
#include <pthread.h>
#include <stdbool.h>
#include "select_event.h"

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 10
typedef struct epoll_event epoll_event;

typedef struct select_event select_event;

select_event * selectEvent;


void on_connection(int fd, void * args){
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof( client_address );
    int connfd = accept( fd, ( struct sockaddr* )&client_address,
                         &client_addrlength );
    char * connection = "new";
    select_event_add( selectEvent,connfd,EPOLLIN | EPOLLET,&on_message,connection)

}

void on_message(int fd,void * args){
    char buf[ BUFFER_SIZE ];
    /* 这些代码不会被重复触发，所以我们的读取数据，以确保把socket 读缓存中的所有数据读出 */
    printf("event trigger once \n");
    while( 1 )
    {
        memset( buf, '\0', BUFFER_SIZE );
        int ret = recv( fd, buf, BUFFER_SIZE-1, 0 );
        if( ret < 0 )
        {
            memset( buf, '\0', BUFFER_SIZE );
            int ret = recv( fd, buf, BUFFER_SIZE-1, 0 );
            if( ret < 0 )
            {
                /* 对于非阻塞IO， 下面的条件成立表示数据已经全部读取完毕。此后
                epoll 就能再次触发 sockfd 上的EPOOLIN事件，以驱动下一次读操作 */
                if( ( errno == EAGAIN ) || ( errno == EWOULDBLOCK ) )
                {
                    printf("read later\n");
                    break;
                }
                close( fd );
                break;
            }
            else if ( ret == 0 )
            {
                close( fd );
            }
            else
            {
                printf("get %d bytes of content: %s\n", ret, buf );
            }
        }
    }
}

int main( int argc, char* argv[] )
{
    if( argc <= 2 )
    {
        printf("usage: %s ip_address port_number\n", basename( argv[0] ));
    }
    const char* ip = argv[1];
    int port = atoi( argv[2] );
    int ret = 0;
    struct sockaddr_in address;
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );
    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
    assert( listenfd >= 0 );
    ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
    assert( ret != -1 );
    ret =  listen( listenfd, 5 );
    assert( ret != -1 );
    create_select_event(selectEvent);
    select_event_add(selectEvent,listenfd,EPOLLIN,&on_connection,listenfd);
    select_loop(selectEvent);
}
