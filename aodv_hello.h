//
// Created by ByteDance on 2022/7/27.
//

#ifndef AODV_ROUTE_AODV_HELLO_H
#define AODV_ROUTE_AODV_HELLO_H

#ifndef NS_NO_GLOBALS
#include "defs.h"
#include "aodv_rrep.h"
#include "routing_table.h"
#endif				/* NS_NO_GLOBALS */

#ifndef NS_NO_DECLARATIONS

#define ROUTE_TIMEOUT_SLACK 100
#define JITTER_INTERVAL 100

void hello_start();
void hello_stop();
void hello_send(void *arg);
void hello_process(RREP * hello, int rreplen, unsigned int ifindex);
void hello_process_non_hello(AODV_msg * aodv_msg, struct in_addr source,
                             unsigned int ifindex);
NS_INLINE void hello_update_timeout(rt_table_t * rt, struct timeval *now,
                                    long time);

#ifdef NS_PORT
long hello_jitter();
#endif
#endif				/* NS_NO_DECLARATIONS */


#endif //AODV_ROUTE_AODV_HELLO_H
