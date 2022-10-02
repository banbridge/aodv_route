//
// Created by ByteDance on 2022/7/27.
//

#ifndef AODV_ROUTE_AODV_RREQ_H
#define AODV_ROUTE_AODV_RREQ_H



#include "route_endian.h"
#include "route_defs.h"
#include "route_seek_list.h"
#include "route_routing_table.h"
#include "route_timer_queue.h"

/* RREQ Flags: */
#define RREQ_JOIN          0x1
#define RREQ_REPAIR        0x2
#define RREQ_GRATUITOUS    0x4
#define RREQ_DEST_ONLY     0x8

typedef struct {
    u_int8_t type;
#if defined(__LITTLE_ENDIAN)
    u_int8_t res1:4;
    u_int8_t d:1;
    u_int8_t g:1;
    u_int8_t r:1;
    u_int8_t j:1;
#elif defined(__BIG_ENDIAN)
    u_int8_t j:1;		/* Join flag (multicast) */
    u_int8_t r:1;		/* Repair flag */
    u_int8_t g:1;		/* Gratuitous RREP flag */
    u_int8_t d:1;		/* Destination only respond */
    u_int8_t res1:4;
#else

#endif
    u_int8_t res2;
    u_int8_t hcnt;
    u_int32_t rreq_id;
    u_int32_t dest_addr;
    u_int32_t dest_seqno;
    u_int32_t orig_addr;
    u_int32_t orig_seqno;
} RREQ;


#define RREQ_SIZE sizeof(RREQ)

/* A data structure to buffer information about received RREQ's */
struct rreq_record {
    ListT l;
    struct in_addr orig_addr;	/* Source of the RREQ */
    u_int32_t rreq_id;		/* RREQ's broadcast ID */
    struct Timer rec_timer;
};

struct blacklist {
    ListT l;
    struct in_addr dest_addr;
    struct Timer bl_timer;
};



RREQ *RreqCreate(u_int8_t flags, struct in_addr dest_addr,
                  u_int32_t dest_seqno, struct in_addr orig_addr);
void RreqSend(struct in_addr dest_addr, u_int32_t dest_seqno, int ttl,
               u_int8_t flags);
void RreqForward(RREQ * rreq, int size, int ttl);
void RreqProcess(RREQ * rreq, int rreqlen, struct in_addr ip_src,
                  struct in_addr ip_dst, int ip_ttl, unsigned int ifindex);
void RreqRouteDiscovery(struct in_addr dest_addr, u_int8_t flags,
                          struct IpData *ipd);
void RreqRecordTimeout(void *arg);
struct blacklist *RreqBlacklistInsert(struct in_addr dest_addr);
void RreqBlacklistTimeout(void *arg);
void RreqLocalRepair(RtTableT * rt, struct in_addr src_addr,
                       struct IpData *ipd);



#endif //AODV_ROUTE_AODV_RREQ_H
