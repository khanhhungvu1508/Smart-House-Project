#include "common.h"
#include "stdint.h"

static int timer_count0 = 0, timer_count1 = 0;
static uint8_t timer_flag0 = 0, timer_flag1 = 0;

void setTimer0(int duration){
    timer_count0 = duration / TIMER_CYCLE;
    timer_flag0 = 0;
}

void setTimer1(int duration){
    timer_count1 = duration / TIMER_CYCLE;
    timer_flag1 = 0;
}

uint8_t getTimerFlag0(){
    return timer_flag0;
}

uint8_t getTimerFlag1(){
    return timer_flag1;
}

void timer_run(){
    if(timer_count0 > 0){
        timer_count0--;
        if(timer_count0 == 0) timer_flag0 = 1;
    }
    if(timer_count1 > 0){
        timer_count1--;
        if(timer_count1 == 0) timer_flag1 = 1;
    }
}
