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
#include "sys/time.h"

#include "defs.h"
#include "list.h"

#ifdef NS_PORT
typedef void (AODVUU::*timeout_func_t) (void *);
#else
typedef void (*timeout_func_t) (void *);
#endif /* NS_PORT */


struct timer{
    list_t l; //链表域
    int used; //主要是用来记录该timer是否已经进入队列，当我们在添加一个timer的时候，如果它已经进入了队列，首先要把它删除，然后再添加
    struct timeval timeout; //记录时间的 timeval 变量
    timeout_func_t handler; //记录时间到了的时候调用的处理函数指针（类型为 timeout_func_t）
    void *data; //一个void类型的指针，存储一些handler需要的数据
};

static inline long timeval_diff(struct timeval *t1, struct timeval *t2){
    long long res;/* We need this to avoid overflows while calculating... */
    if (!t1 || !t2){
        return -1;
    }else{
        res = t1->tv_sec;
        res = ((res - t2->tv_sec) * 1000000 + t1->tv_usec - t2->tv_usec) / 1000;
        return (long)res;
    }
}

static inline int timeval_add_msec(struct timeval *t, unsigned long msec){
    unsigned long long add;

    if(!t){
        return -1;
    }

    add = t->tv_usec + (msec*1000);
    t->tv_sec += add/100000;
    t->tv_usec = add%1000000;
    return 0;
}
#endif				/* NS_NO_GLOBALS */

#ifndef NS_NO_DECLARATIONS
//未实现（不过在 timer_queue.c 里面定义了一个链表头TQ）
void timer_queue_init();
//在TQ链表中删除指定timer
int timer_remove(struct timer *t);
//改变某个timer的timeout（如果这个timer已经进入队列也就是used了，首先要移出）
void timer_set_timeout(struct timer *t, long msec);
int timer_timeout_now(struct timer *t);
//首先把当前时间（now）之前的timer全部到时（也就是调用timer_timeout(&now)），然后返回下一个即将到期的timer剩下的时间
struct timeval *timer_age_queue();
/* timer_init should be called for every newly allocated timer */
//用来初始化一个timer（绑定处理函数等等）
int timer_init(struct timer *t, timeout_func_t f, void *data);

#ifdef NS_PORT
//如果已经used，首先删除，然后添加，在添加的时候遍历整个TQ，找到合适的位置插入（保证时序）
void timer_add(struct timer *t);

//当一个timer到时的时候调用的函数，作用是把某个时间前的所有timer都移除，并且调用他们的处理函数
void timer_timeout(struct timeval *now);

#ifdef DEBUG_TIMER_QUEUE
void NS_CLASS printTQ();
#endif				/* DEBUG_TIMER_QUEUE */

#endif				/* NS_PORT */

#endif				/* NS_NO_DECLARATIONS */


#endif //AODV_ROUTE_TIMER_QUEUE_H
