/*
 * @Author: 高逸博 gaoyibo.09@bytedance.com
 * @Date: 2022-07-27 08:46:57
 * @LastEditors: 高逸博 gaoyibo.09@bytedance.com
 * @LastEditTime: 2022-07-27 11:30:42
 * @FilePath: /aodv-route/seek_list.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
//
// Created by ByteDance on 2022/7/27.
//

#include "route_seek_list.h"

#include "stdlib.h"

#ifdef NS_PORT
#include "ns-2/aodv-uu.h"
#include "list.h"
#else
#include "route_seek_list.h"
#include "route_timer_queue.h"
#include "route_timeout.h"
#include "route_defs.h"
#include "route_params.h"
#include "route_debug.h"
#include "route_list.h"
#endif

#ifndef NS_PORT
/* The seek list is a linked list of destinations we are seeking
   (with RREQ's). */

static LIST(seekhead);

#ifdef SEEK_LIST_DEBUG
void seek_list_print();
#endif

SeekListT *NS_CLASS SeekListInsert(struct in_addr dest_addr,
                           u_int32_t dest_seqno,
                           int ttl, u_int8_t flags,
                           struct IpData *ipd ){

    SeekListT *entry;

    if ((entry = (SeekListT *) malloc(sizeof(SeekListT))) == NULL) {
	fprintf(stderr, "Failed malloc\n");
	exit(-1);
    }

    entry->dest_addr = dest_addr;
    entry->dest_seqno = dest_seqno;
    entry->flags = flags;
    entry->reqs = 0;
    entry->ttl = ttl;
    entry->ipd = ipd;

    TimerInit(&entry->seek_timer, &NS_CLASS route_discovery_timeout, entry);

    ListAdd(&seekhead, &entry->l);
#ifdef SEEK_LIST_DEBUG
    seek_list_print();
#endif
    return entry;
 }

int NS_CLASS SeekListRemove(seek_list_t * entry){
    if (!entry)
	return 0;

    ListDetach(&entry->l);

    /* Make sure any timers are removed */
    TimerRemove(&entry->seek_timer);

    if (entry->ipd)
	free(entry->ipd);

    free(entry);
    return 1;
}

SeekListT *NS_CLASS SeekListFind(struct in_addr dest_addr){
    ListT *pos;

    ListForeach(pos, &seekhead) {
        SeekListT *entry = (SeekListT *) pos;

	if (entry->dest_addr.s_addr == dest_addr.s_addr)
	    return entry;
    }
    return NULL;
}

#endif				/* NS_PORT */


#ifdef SEEK_LIST_DEBUG
void NS_CLASS seek_list_print(){
    list_t *pos;

    list_foreach(pos, &seekhead) {
	seek_list_t *entry = (seek_list_t *) pos;
	printf("%s %u %d %d\n", ip_to_str(entry->dest_addr),
	       entry->dest_seqno, entry->reqs, entry->ttl);
    }
}
#endif