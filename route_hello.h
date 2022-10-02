//
// Created by ByteDance on 2022/7/27.
//

#ifndef AODV_ROUTE_AODV_HELLO_H
#define AODV_ROUTE_AODV_HELLO_H

#include "route_defs.h"
#include "route_rrep.h"
#include "route_routing_table.h"


#define ROUTE_TIMEOUT_SLACK 100
#define JITTER_INTERVAL 100

void HelloStart();
void HelloStop();
void HelloSend(void *arg);
void HelloProcess(RREP * hello, int rreplen, unsigned int ifindex);
void HelloProcessNonHello(AODV_msg * aodv_msg, struct in_addr source,
                             unsigned int ifindex);
NS_INLINE void HelloUpdateTimeout(RtTableT * rt, struct timeval *now,
                                    long time);


#endif //AODV_ROUTE_AODV_HELLO_H
