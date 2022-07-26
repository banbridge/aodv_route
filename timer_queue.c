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