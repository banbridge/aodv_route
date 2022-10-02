//
// Created by ByteDance on 2022/7/25.
//

/*
 * 总览整个 timer_queue 的功能，其实就是一个离散事件模拟器，
 * 用一个链表记录即将到来的事件，然后对相应的事件调用相应的处理函数，
 * 需要计时的比如hello消息的统计都会通过这个方式来完成。另外里面还有一
 * 个 gettimeofday函数，用来获取当前时间，它实际上是获取了调度器
 * Scheduler的时钟。
 */

#ifndef AODV_ROUTE_TIMER_QUEUE_H
#define AODV_ROUTE_TIMER_QUEUE_H

#ifndef NS_NO_GLOBALS
#include <sys/time.h>

#include "route_defs.h"
#include "route_list.h"

typedef void (*timeout_func_t) (void *);



struct Timer{
    ListT l; //链表域
    int used; //主要是用来记录该timer是否已经进入队列，当我们在添加一个timer的时候，如果它已经进入了队列，首先要把它删除，然后再添加
    struct timeval timeout; //记录时间的 timeval 变量
    timeout_func_t handler; //记录时间到了的时候调用的处理函数指针（类型为 timeout_func_t）
    void *data; //一个void类型的指针，存储一些handler需要的数据
};

static inline long TimevalDiff(struct timeval *t1, struct timeval *t2){
    long long res;/* We need this to avoid overflows while calculating... */
    if (!t1 || !t2){
        return -1;
    }else{
        res = t1->tv_sec;
        res = ((res - t2->tv_sec) * 1000000 + t1->tv_usec - t2->tv_usec) / 1000;
        return (long)res;
    }
}

static inline int TimevalAddMsec(struct timeval *t, unsigned long msec){
    unsigned long long add;

    if(!t){
        return -1;
    }

    add = t->tv_usec + (msec*1000);
    t->tv_sec += add/1000000;
    t->tv_usec = add%1000000;
    return 0;
}
#endif				/* NS_NO_GLOBALS */

#ifndef NS_NO_DECLARATIONS
//未实现（不过在 timer_queue.c 里面定义了一个链表头TQ）
void TimerQueueInit();
//在TQ链表中删除指定timer
int TimerRemove(struct Timer *t);
//改变某个timer的timeout（如果这个timer已经进入队列也就是used了，首先要移出）
void TimerSetTimeout(struct Timer *t, long msec);
int TimerTimeoutNow(struct Timer *t);
//首先把当前时间（now）之前的timer全部到时（也就是调用timer_timeout(&now)），然后返回下一个即将到期的timer剩下的时间
struct timeval *TimerAgeQueue();
/* timer_init should be called for every newly allocated timer */
//用来初始化一个timer（绑定处理函数等等）
int TimerInit(struct Timer *t, timeout_func_t f, void *data);

#endif				/* NS_NO_DECLARATIONS */


#endif //AODV_ROUTE_TIMER_QUEUE_H
