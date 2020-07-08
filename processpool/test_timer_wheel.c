#include "magox.h"
#include "select_event.h"
#include <sys/timerfd.h>
#include "Timer/timer_factory.h"

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
    int ret = 0;
    map_event_item_t * events;
    selectEvent = create_select_event(events);
    TimerInterface * mytimer = build_timer(TIMER_WHEEL);
    for(int i=0; i<3;i++){
        mytimer->add(mytimer,i*10,i,timer_call_handle,NULL);
    }
    mytimer->start(mytimer);

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

    timer_wheel * timerWheel;
    timerWheel = get_thiz_parent(timer_wheel,timerInterface,mytimer);
    select_event_add(selectEvent,timefd,EPOLLIN,timer_tick,  timerWheel);
    select_loop(selectEvent);
}
