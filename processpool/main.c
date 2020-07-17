#include "magox.h"
#include "select_event.h"
#include <sys/timerfd.h>
#include "Timer/timer_factory.h"
#include "tcp_connection.h"


#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 60000
typedef struct epoll_event epoll_event;

#define handle_error(msg) \
       do { perror(msg); exit(EXIT_FAILURE); } while (0)


select_event * selectEvent;

static void
print_elapsed_time(void){
    static struct timespec start;
    struct timespec curr;
    static int first_call = 1;
    int secs, nsecs;
    if (first_call) {
        first_call = 0;
        if (clock_gettime(CLOCK_MONOTONIC, &start) == -1)
            handle_error("clock_gettime");
    }

    if (clock_gettime(CLOCK_MONOTONIC, &curr) == -1)
        handle_error("clock_gettime");

    secs = curr.tv_sec - start.tv_sec;
    nsecs = curr.tv_nsec - start.tv_nsec;
    if (nsecs < 0) {
        secs--;
        nsecs += 1000000000;
    }
    printf("%d.%03d: ", secs, (nsecs + 500000) / 1000000);
}

void on_message(ConnectionInterface * conn,void * request){
    char *buffer;
    buffer="HTTP/1.1 200 OK\r\nServer: swoole-http-server\r\nContent-Type: text/html\r\nContent-Length: 37\r\n\r\n<html><body>hello world</body></html>\r\n\r\n";;
    conn->close(conn ,buffer);
}

void on_connection(int fd,void *args){
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof( client_address );
    int connfd = accept( fd, ( struct sockaddr* )&client_address,
                         &client_addrlength );
    connection_mannege * connectionMannege =  create_connection_mannege();
    tcp_connection * tcpConnection =  new_tcp_connection(connectionMannege,connfd,HTTP_PROTOCOL,on_message,args);
    connection_mannage_add(connectionMannege,tcpConnection);
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
    select_event_add(selectEvent,listenfd,EPOLLIN,on_connection,selectEvent);
    select_loop(selectEvent);
}
