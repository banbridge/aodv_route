//
// Created by ByteDance on 2022/7/25.
//

#ifndef AODV_ROUTE_LIST_H
#define AODV_ROUTE_LIST_H

/* Simple linked list inspired from the Linux kernel list implementation */
typedef struct ListT{
    struct ListT *prev, *next;
} ListT;

#define LIST_NULL -1
#define LIST_SUCCESS 1

#define LIST(name) ListT name = { &(name), (&name)}

#define INIT_LIST_HEAD(h) do { \
	(h)->next = (h); (h)->prev = (h); \
} while (0)

#define INIT_LIST_ELM(le) do { \
	(le)->next = NULL; (le)->prev = NULL; \
} while (0)

int ListDetach(ListT * le);
int ListAddTail(ListT * head, ListT * le);
int ListAdd(ListT * head, ListT * le);

#define ListForeach(curr, head) \
        for (curr = (head)->next; curr != (head); curr = curr->next)

#define ListForeachSafe(pos, tmp, head) \
        for (pos = (head)->next, tmp = pos->next; pos != (head); \
                pos = tmp, tmp = pos->next)

#define ListEmpty(head) ((head) == (head)->next)

#define ListFirst(head) ((head)->next)

#define ListUnattached(le) ((le)->next == NULL && (le)->prev == NULL)

#endif //AODV_ROUTE_LIST_H
