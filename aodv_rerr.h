//
// Created by ByteDance on 2022/7/27.
//

#ifndef AODV_ROUTE_AODV_RERR_H
#define AODV_ROUTE_AODV_RERR_H


#ifndef NS_NO_GLOBALS
#include "endian_sell.h"

#include "defs.h"
#include "routing_table.h"

/* RERR Flags: */
#define RERR_NODELETE 0x1

typedef struct {
    u_int8_t type;
#if defined(__LITTLE_ENDIAN)
    u_int8_t res1:7;
    u_int8_t n:1;
#elif defined(__BIG_ENDIAN)
    u_int8_t n:1;
    u_int8_t res1:7;
#else
#error "Adjust your <bits/endian.h> defines"
#endif
    u_int8_t res2;
    u_int8_t dest_count;
    u_int32_t dest_addr;
    u_int32_t dest_seqno;
} RERR;

#define RERR_SIZE sizeof(RERR)

/* Extra unreachable destinations... */
typedef struct {
    u_int32_t dest_addr;
    u_int32_t dest_seqno;
} RERR_udest;

#define RERR_UDEST_SIZE sizeof(RERR_udest)

/* Given the total number of unreachable destination this macro
   returns the RERR size */
#define RERR_CALC_SIZE(rerr) (RERR_SIZE + (rerr->dest_count-1)*RERR_UDEST_SIZE)
#define RERR_UDEST_FIRST(rerr) ((RERR_udest *)&rerr->dest_addr)
#define RERR_UDEST_NEXT(udest) ((RERR_udest *)((char *)udest + RERR_UDEST_SIZE))
#endif				/* NS_NO_GLOBALS */

#ifndef NS_NO_DECLARATIONS
RERR *rerr_create(u_int8_t flags, struct in_addr dest_addr,
                  u_int32_t dest_seqno);
void rerr_add_udest(RERR * rerr, struct in_addr udest, u_int32_t udest_seqno);
void rerr_process(RERR * rerr, int rerrlen, struct in_addr ip_src,
                  struct in_addr ip_dst);
#endif				/* NS_NO_DECLARATIONS */


#endif //AODV_ROUTE_AODV_RERR_H
