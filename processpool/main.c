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
#include <sys/timerfd.h>
#include "timer_wheel.h"

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

void timer_call_handle(int fd,void * args){
    printf("time_done: %d \n", fd);
}

void timer_tick(int fd,void * args){
    uint64_t exp, tot_exp;
    ssize_t s;
    s = read(fd, &exp, sizeof(uint64_t));
    if (s != sizeof(uint64_t))
        handle_error("read");
    tot_exp += exp;
    print_elapsed_time();
    printf("read: %llu; total=%llu\n",
           (unsigned long long) exp,
           (unsigned long long) tot_exp);
    wheel_tick(args);
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
    timer_wheel * timerWheel = create_timer_wheel (1,10);
    TimerInterface * thiz_timer = get_thiz_by_timer_wheel(timerWheel);
    for(int i=0; i<3;i++){
        thiz_timer->add(thiz_timer,i*10,i,timer_call_handle,NULL);
    }
    wheel_start(timerWheel);

    struct itimerspec new_value;
    struct timespec now;
    uint64_t exp;
    ssize_t s;
    ret = clock_gettime(CLOCK_REALTIME, &now);//获取时钟时间
    assert(ret != -1);
    new_value.it_value.tv_sec = 3; //第一次到期的时间
    new_value.it_value.tv_nsec = now.tv_nsec;
    new_value.it_interval.tv_sec = 1;      //之后每次到期的时间间隔
    new_value.it_interval.tv_nsec = 0;
    int timefd = timerfd_create(CLOCK_REALTIME, 0); // 构建了一个定时器
    assert(timefd != -1);
    ret = timerfd_settime(timefd, 0, &new_value, NULL);//启动定时器
    assert(ret != -1);

    select_event_add(selectEvent,timefd,EPOLLIN,timer_tick,timerWheel);

    select_loop(selectEvent);
}
