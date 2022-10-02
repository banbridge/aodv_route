#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>


#include "route_timer_queue.h"
#include "route_defs.h"
#include "route_debug.h"
#include "route_list.h"

static LIST(TQ);

/* #define DEBUG_TIMER_QUEUE */

#ifdef DEBUG_TIMER_QUEUE
static void printTQ(list_t * l);
#endif

/*
 * 用来初始化一个timer（绑定处理函数等等）
 */
int NS_CLASS TimerInit(struct Timer *t, timeout_func_t f, void *data) {

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
void NS_CLASS TimerTimeout(struct timeval *now) {
    LIST(expTQ);
    list_t *pos, *tmp;

#ifdef DEBUG_TIMER_QUEUE
    printf("\n######## timer_timeout: called!!\n");
#endif
    /* Remove expired timers from TQ and add them to expTQ */
    ListForeachSafe(pos, tmp, &TQ) {
        struct Timer *t = (struct Timer *) pos;

        if (TimevalDiff(&t->timeout, now) > 0)
            break;

        ListDetach(&t->l);
        ListAddTail(&expTQ, &t->l);
    }

    /* Execute expired timers in expTQ safely by removing them at the head */
    while (!ListEmpty(&expTQ)) {
        struct Timer *t = (struct Timer *) ListFirst(&expTQ);
        ListDetach(&t->l);
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
NS_STATIC void NS_CLASS TimerAdd(struct Timer *t) {
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
        TimerRemove(t);

    t->used = 1;

#ifdef DEBUG_TIMER_QUEUE
    printf("New timer added!\n");
#endif

    /* Base case when queue is empty: */
    if (ListEmpty(&TQ)) {
        ListAdd(&TQ, &t->l);
    } else {

        ListForeach(pos, &TQ) {
            struct Timer *curr = (struct Timer *) pos;
            if (TimevalDiff(&t->timeout, &curr->timeout) < 0) {
                break;
            }
        }
        ListAdd(pos->prev, &t->l);
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
int NS_CLASS TimerRemove(struct Timer *t) {
    int res = 1;

    if (!t)
        return -1;


    if (ListUnattached(&t->l))
        res = 0;
    else
        ListDetach(&t->l);

    t->used = 0;

    return res;
}

/**
 * 删除当前timer，然后调用处理函数（不知道这个有啥用）
 * @param t
 * @return
 */
int NS_CLASS TimerTimeoutNow(struct Timer *t) {
    if (TimerRemove(t)) {


        t->handler(t->data);
        return 1;
    }
    return -1;
}

/**
 * 改变某个timer的timeout（如果这个timer已经进入队列也就是used了，首先要移出）
 * @param t
 * @param msec
 */
void NS_CLASS TimerSetTimeout(struct Timer *t, long msec) {
    if (t->used) {
        TimerRemove(t);
    }

    gettimeofday(&t->timeout, NULL);

    if (msec < 0)
            DEBUG(LOG_WARNING, 0, "Negative timeout!!!");

    t->timeout.tv_usec += msec * 1000;
    t->timeout.tv_sec += t->timeout.tv_usec / 1000000;
    t->timeout.tv_usec = t->timeout.tv_usec % 1000000;

    TimerAdd(t);
}

long TimerLeft(struct Timer *t) {
    struct timeval now;

    if (!t)
        return -1;

    gettimeofday(&now, NULL);

    return TimevalDiff(&now, &t->timeout);
}

/**
 * 首先把当前时间（now）之前的timer全部到时（也就是调用timer_timeout(&now)），然后返回下一个即将到期的timer剩下的时间
 * @return
 */
struct timeval *NS_CLASS TimerAgeQueue() {
    struct timeval now;
    struct Timer *t;
    static struct timeval remaining;

    gettimeofday(&now, NULL);

    fflush(stdout);

    if (ListEmpty(&TQ))
        return NULL;

    TimerTimeout(&now);

    /* Check emptyness again since the list might have been updated by a
     * timeout */
    if (ListEmpty(&TQ))
        return NULL;

    t = (struct Timer *) TQ.next;

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

    ListForeach(pos, l) {
    struct Timer *t = (struct Timer *) pos;
    fprintf(stderr, "%-12ld %-4d %lu\n", TimevalDiff(&t->timeout, &now), n,
        (unsigned long) pos);
    n++;
    }
}
#endif
