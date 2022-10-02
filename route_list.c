//
// Created by ByteDance on 2022/7/25.
//

#include <stdlib.h>

#include "route_list.h"

static inline int ListelmDetach(ListT *pre, ListT *next) {
    next->prev = pre;
    pre->next = next;
    return LIST_SUCCESS;
}

static inline int ListelmAdd(ListT *le, ListT *prev, ListT *next) {
    prev->next = le;
    le->prev = prev;
    le->next = next;
    next->prev = le;

    return LIST_SUCCESS;
}

int ListAdd(ListT * head, ListT * le)
{

    if (!head || !le)
        return LIST_NULL;

    ListelmAdd(le, head, head->next);

    return LIST_SUCCESS;
}

int ListAddTail(ListT * head, ListT * le)
{

    if (!head || !le)
        return LIST_NULL;

    ListelmAdd(le, head->prev, head);

    return LIST_SUCCESS;
}

int ListDetach(ListT * le)
{
    if (!le)
        return LIST_NULL;

    ListelmDetach(le->prev, le->next);

    le->next = le->prev = NULL;

    return LIST_SUCCESS;
}
