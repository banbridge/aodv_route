//
// Created by ByteDance on 2022/7/27.
//

#ifndef AODV_ROUTE_AODV_RREP_H
#define AODV_ROUTE_AODV_RREP_H


#include "route_endian.h"
#include "route_defs.h"
#include "route_routing_table.h"

/* RREP Flags: */

#define RREP_ACK       0x1
#define RREP_REPAIR    0x2

typedef struct {
    u_int8_t type;
#if defined(__LITTLE_ENDIAN)
    u_int16_t res1:6;
    u_int16_t a:1;
    u_int16_t r:1;
    u_int16_t prefix:5;
    u_int16_t res2:3;
#elif defined(__BIG_ENDIAN)
    u_int16_t r:1;
    u_int16_t a:1;
    u_int16_t res1:6;
    u_int16_t res2:3;
    u_int16_t prefix:5;
#else
#error "Adjust your <bits/endian.h> defines"
#endif
    u_int8_t hcnt;
    u_int32_t dest_addr;
    u_int32_t dest_seqno;
    u_int32_t orig_addr;
    u_int32_t lifetime;
} RREP;

#define RREP_SIZE sizeof(RREP)

typedef struct {
    u_int8_t type;
    u_int8_t reserved;
} RREP_ack;

#define RREP_ACK_SIZE sizeof(RREP_ack)


RREP *RrepCreate(u_int8_t flags,
                  u_int8_t prefix,
                  u_int8_t hcnt,
                  struct in_addr dest_addr,
                  u_int32_t dest_seqno,
                  struct in_addr orig_addr, u_int32_t life);

RREP_ack *RrepAckCreate();
AODV_ext *RrepAddExt(RREP * rrep, int type, unsigned int offset,
                       int len, char *data);
void RrepSend(RREP * rrep, RtTableT * rev_rt, RtTableT * fwd_rt, int size);
void RrepForward(RREP * rrep, int size, RtTableT * rev_rt,
                  RtTableT * fwd_rt, int ttl);
void RrepProcess(RREP * rrep, int rreplen, struct in_addr ip_src,
                  struct in_addr ip_dst, int ip_ttl, unsigned int ifindex);
void RrepAckProcess(RREP_ack * rrep_ack, int rreplen, struct in_addr ip_src,
                      struct in_addr ip_dst);



#endif //AODV_ROUTE_AODV_RREP_H
