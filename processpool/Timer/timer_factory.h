//
// Created by magox on 7/8/20.
//

#ifndef PROCESSPOOL_TIMER_FACTORY_H
#define PROCESSPOOL_TIMER_FACTORY_H
#include "timer_wheel.h"


enum {
    TIMER_WHEEL = 1
};

static inline TimerInterface * build_timer(int type){
    TimerInterface * timer;
    if(type==TIMER_WHEEL){
        timer_wheel * timerWheel = create_timer_wheel (1,10);
        timer = get_thiz(timer_wheel,TimerInterface ,timerInterface,timerWheel);
    }
    return timer;
}
#endif //PROCESSPOOL_TIMER_FACTORY_H
