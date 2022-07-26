//
// Created by ByteDance on 2022/7/26.
//

#include "stdlib.h"
#include "signal.h"
#include "sys/time.h"

#ifdef NS_PORT
#include "ns-2/aodv-uu.h"
#else

#include "timer_queue.h"
#include "defs.h"
#include "debug.h"
#include "list.h"

static LIST(TQ);

/* #define DEBUG_TIMER_QUEUE */

#ifdef DEBUG_TIMER_QUEUE
static void printTQ(list_t * l);
#endif
#endif                /* NS_PORT */

/*
 * 用来初始化一个timer（绑定处理函数等等）
 */
int NS_CLASS timer_init(struct timer *t, timeout_func_t f, void *data) {

    if (!t) {
        return -1;
    }

    INIT_LIST_ELM(&t->l);
    t->handler = f;
    t->data = data;
    t->timeout.tv_sec = 0;
    t->timeout.tv_usec = 0;
    return 0;
}

/* Called when a timer should timeout */
/**
 * //当一个timer到时的时候调用的函数，作用是把某个时间前的所有timer都移除，并且调用他们的处理函数
 * @param now
 */
void NS_CLASS timer_timeout(struct timeval *now) {
    LIST(expTQ);
    list_t *pos, *tmp;

#ifdef DEBUG_TIMER_QUEUE
    printf("\n######## timer_timeout: called!!\n");
#endif
    /* Remove expired timers from TQ and add them to expTQ */
    list_foreach_safe(pos, tmp, &TQ) {
        struct timer *t = (struct timer *) pos;

        if (timeval_diff(&t->timeout, now) > 0)
            break;

        list_detach(&t->l);
        list_add_tail(&expTQ, &t->l);
    }

    /* Execute expired timers in expTQ safely by removing them at the head */
    while (!list_empty(&expTQ)) {
        struct timer *t = (struct timer *) list_first(&expTQ);
        list_detach(&t->l);
        t->used = 0;
#ifdef DEBUG_TIMER_QUEUE
        printf("removing timer %lu %d\n", pos);
#endif
        /* Execute handler function for expired timer... */
        if (t->handler) {
#ifdef NS_PORT
            (*this.*t->handler) (t->data);
#else
            t->handler(t->data);
#endif
        }
    }
}

/**
 * 如果已经used，首先删除，然后添加，在添加的时候遍历整个TQ，找到合适的位置插入（保证时序）
 * @param t
 */
NS_STATIC void NS_CLASS timer_add(struct timer *t) {
    list_t *pos;

    /* Sanity checks: */
    if (!t) {
        perror("NULL timer!!!\n");
        exit(-1);
    }
    if (!t->handler) {
        perror("NULL handler!!!\n");
        exit(-1);
    }

    /* Make sure we remove unexpired timers before adding a new timeout... */
    if (t->used)
        timer_remove(t);

    t->used = 1;

#ifdef DEBUG_TIMER_QUEUE
    printf("New timer added!\n");
#endif

    /* Base case when queue is empty: */
    if (list_empty(&TQ)) {
        list_add(&TQ, &t->l);
    } else {

        list_foreach(pos, &TQ) {
            struct timer *curr = (struct timer *) pos;
            if (timeval_diff(&t->timeout, &curr->timeout) < 0) {
                break;
            }
        }
        list_add(pos->prev, &t->l);
    }

#ifdef DEBUG_TIMER_QUEUE
    printTQ(&TQ);
#endif

}

/**
 * 在TQ链表中删除指定timer
 * @param t
 * @return
 */
int NS_CLASS timer_remove(struct timer *t) {
    int res = 1;

    if (!t)
        return -1;


    if (list_unattached(&t->l))
        res = 0;
    else
        list_detach(&t->l);

    t->used = 0;

    return res;
}

/**
 * 删除当前timer，然后调用处理函数（不知道这个有啥用）
 * @param t
 * @return
 */
int NS_CLASS timer_timeout_now(struct timer *t) {
    if (timer_remove(t)) {

#ifdef NS_PORT
        (*this.*t->handler) (t->data);
#else
        t->handler(t->data);
#endif
        return 1;
    }
    return -1;
}

/**
 * 改变某个timer的timeout（如果这个timer已经进入队列也就是used了，首先要移出）
 * @param t
 * @param msec
 */
void NS_CLASS timer_set_timeout(struct timer *t, long msec) {
    if (t->used) {
        timer_remove(t);
    }

    gettimeofday(&t->timeout, NULL);

    if (msec < 0)
            DEBUG(LOG_WARNING, 0, "Negative timeout!!!");

    t->timeout.tv_usec += msec * 1000;
    t->timeout.tv_sec += t->timeout.tv_usec / 1000000;
    t->timeout.tv_usec = t->timeout.tv_usec % 1000000;

    timer_add(t);
}

long timer_left(struct timer *t) {
    struct timeval now;

    if (!t)
        return -1;

    gettimeofday(&now, NULL);

    return timeval_diff(&now, &t->timeout);
}

/**
 * 首先把当前时间（now）之前的timer全部到时（也就是调用timer_timeout(&now)），然后返回下一个即将到期的timer剩下的时间
 * @return
 */
struct timeval *NS_CLASS timer_age_queue() {
    struct timeval now;
    struct timer *t;
    static struct timeval remaining;

    gettimeofday(&now, NULL);

    fflush(stdout);

    if (list_empty(&TQ))
        return NULL;

    timer_timeout(&now);

    /* Check emptyness again since the list might have been updated by a
     * timeout */
    if (list_empty(&TQ))
        return NULL;

    t = (struct timer *) TQ.next;

    remaining.tv_usec = (t->timeout.tv_usec - now.tv_usec);
    remaining.tv_sec = (t->timeout.tv_sec - now.tv_sec);

    if (remaining.tv_usec < 0) {
        remaining.tv_usec += 1000000;
        remaining.tv_sec -= 1;
    }
    return (&remaining);
}


#ifdef DEBUG_TIMER_QUEUE
void NS_CLASS printTQ(list_t * l){
    struct timeval now;
    int n = 0;
    list_t *pos;

    gettimeofday(&now, NULL);

    fprintf(stderr, "================\n");
    fprintf(stderr, "%-12s %-4s %lu\n", "left", "n", (unsigned long) l);

    list_foreach(pos, l) {
    struct timer *t = (struct timer *) pos;
    fprintf(stderr, "%-12ld %-4d %lu\n", timeval_diff(&t->timeout, &now), n,
        (unsigned long) pos);
    n++;
    }
}
#endif
