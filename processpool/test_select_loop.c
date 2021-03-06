
#include "select_event.h"

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 60000
typedef struct epoll_event epoll_event;


select_event * selectEvent;


void on_message(int fd,void * args){
    char buf[ BUFFER_SIZE ];
    printf("event trigger once \n");
    while( 1 )
    {
        memset( buf, '\0', BUFFER_SIZE );
        int ret = recv( fd, buf, BUFFER_SIZE-1, 0 );
        send(fd,&buf,BUFFER_SIZE-1,0);
        if( ret < 0 )
        {
            memset( buf, '\0', BUFFER_SIZE );
            int ret = recv( fd, buf, BUFFER_SIZE-1, 0 );
            if( ret < 0 )
            {
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



void on_connection(int fd, void * args){
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof( client_address );
    int connfd = accept( fd, ( struct sockaddr* )&client_address,
                         &client_addrlength );
    char * connection = "new";
    select_event_add(selectEvent,connfd,EPOLLIN,on_message,NULL);
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
    map_event_item_t * events;
    selectEvent = create_select_event(events);
    select_event_add(selectEvent,listenfd,EPOLLIN,on_connection,NULL);
    select_loop(selectEvent);
}
