//
// Created by ByteDance on 2022/7/27.
//

#ifndef AODV_ROUTE_ROUTING_TABLE_H
#define AODV_ROUTE_ROUTING_TABLE_H


#ifndef NS_NO_GLOBALS
#include "route_defs.h"
#include "route_list.h"
#include "route_timer_queue.h"

typedef struct RtTable RtTableT;

/* Neighbor struct for active routes in Route Table */
typedef struct Precursor {
    ListT l;
    struct in_addr neighbor;
} PrecursorT;

#define FIRST_PREC(h) ((PrecursorT *)((h).next))

#define SeqnoIncr(s) ((s == 0) ? 0 : ((s == 0xFFFFFFFF) ? s = 1 : s++))

typedef u_int32_t hash_value;	/* A hash value */

/* Route table entries */
struct RtTable {
    ListT l;
    struct in_addr dest_addr;	/* IP address of the destination */
    u_int32_t dest_seqno;
    unsigned int ifindex;	/* Network interface index... */
    struct in_addr next_hop;	/* IP address of the next hop to the dest */
    u_int8_t hcnt;		/* Distance (in hops) to the destination */
    u_int16_t flags;		/* Routing flags */
    u_int8_t state;		/* The state of this entry */
    struct Timer rt_timer;	/* The timer associated with this entry */
    struct Timer ack_timer;	/* RREP_ack timer for this destination */
    struct Timer hello_timer;
    struct timeval last_hello_time;
    u_int8_t hello_cnt;
    hash_value hash;
    int nprec;			/* Number of precursors */
    ListT precursors;		/* List of neighbors using the route */
};


/* Route entry flags */
#define RT_UNIDIR        0x1
#define RT_REPAIR        0x2
#define RT_INV_SEQNO     0x4
#define RT_INET_DEST     0x8	/* Mark for Internet destinations (to be relayed
				 * through a Internet gateway. */
#define RT_GATEWAY       0x10

/* Route entry states */
#define INVALID   0
#define VALID     1


#define RT_TABLESIZE 64		/* Must be a power of 2 */
#define RT_TABLEMASK (RT_TABLESIZE - 1)

struct RoutingTable {
    unsigned int num_entries;
    unsigned int num_active;
    ListT tbl[RT_TABLESIZE];
};

void PrecursorListDestroy(RtTableT * rt);
#endif				/* NS_NO_GLOBALS */

#ifndef NS_NO_DECLARATIONS

struct RtTable rt_tbl;

void RtTableInit();
void RtTableDestroy();
RtTableT *RtTableInsert(struct in_addr dest, struct in_addr next,
                            u_int8_t hops, u_int32_t seqno, u_int32_t life,
                            u_int8_t state, u_int16_t flags,
                            unsigned int ifindex);
RtTableT *RtTableUpdate(RtTableT * rt, struct in_addr next, u_int8_t hops,
                            u_int32_t seqno, u_int32_t lifetime, u_int8_t state,
                            u_int16_t flags);
NS_INLINE RtTableT *RtTableUpdateTimeout(RtTableT * rt,
                                              u_int32_t lifetime);
void RtTableUpdateRouteTimeouts(RtTableT * fwd_rt, RtTableT * rev_rt);
RtTableT *RtTableFind(struct in_addr dest);
RtTableT *RtTableFindGateway();
int RtTableUpdateInetRt(RtTableT * gw, u_int32_t life);
int RtTableInvalidate(RtTableT * rt);
void RtTableDelete(RtTableT * rt);
void PrecursorAdd(RtTableT * rt, struct in_addr addr);
void PrecursorRemove(RtTableT * rt, struct in_addr addr);

#endif				/* NS_NO_DECLARATIONS */


#endif //AODV_ROUTE_ROUTING_TABLE_H
