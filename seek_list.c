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

#include "seek_list.h"

#include "stdlib.h"

#ifdef NS_PORT
#include "ns-2/aodv-uu.h"
#include "list.h"
#else
#include "seek_list.h"
#include "timer_queue.h"
#include "aodv_timeout.h"
#include "defs.h"
#include "params.h"
#include "debug.h"
#include "list.h"
#endif

#ifndef NS_PORT
/* The seek list is a linked list of destinations we are seeking
   (with RREQ's). */

static LIST(seekhead);

#ifdef SEEK_LIST_DEBUG
void seek_list_print();
#endif

seek_list_t *NS_CLASS seek_list_insert(struct in_addr dest_addr,
                           u_int32_t dest_seqno,
                           int ttl, u_int8_t flags,
                           struct ip_data *ipd ){
    
    seek_list_t *entry;

    if ((entry = (seek_list_t *) malloc(sizeof(seek_list_t))) == NULL) {
	fprintf(stderr, "Failed malloc\n");
	exit(-1);
    }

    entry->dest_addr = dest_addr;
    entry->dest_seqno = dest_seqno;
    entry->flags = flags;
    entry->reqs = 0;
    entry->ttl = ttl;
    entry->ipd = ipd;

    timer_init(&entry->seek_timer, &NS_CLASS route_discovery_timeout, entry);

    list_add(&seekhead, &entry->l);
#ifdef SEEK_LIST_DEBUG
    seek_list_print();
#endif
    return entry;
 }

int NS_CLASS seek_list_remove(seek_list_t * entry){
    if (!entry)
	return 0;

    list_detach(&entry->l);

    /* Make sure any timers are removed */
    timer_remove(&entry->seek_timer);

    if (entry->ipd)
	free(entry->ipd);

    free(entry);
    return 1;
}

seek_list_t *NS_CLASS seek_list_find(struct in_addr dest_addr){
    list_t *pos;

    list_foreach(pos, &seekhead) {
	seek_list_t *entry = (seek_list_t *) pos;

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