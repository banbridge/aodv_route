//
// Created by ByteDance on 2022/7/27.
//

#ifndef AODV_ROUTE_AODV_NEIGHBOR_H
#define AODV_ROUTE_AODV_NEIGHBOR_H

#ifndef NS_NO_GLOBALS
#include "defs.h"
#include "routing_table.h"
#endif				/* NS_NO_GLOBALS */

#ifndef NS_NO_DECLARATIONS

void neighbor_add(AODV_msg * aodv_msg, struct in_addr source,
                  unsigned int ifindex);
void neighbor_link_break(rt_table_t * rt);

#endif				/* NS_NO_DECLARATIONS */


#endif //AODV_ROUTE_AODV_NEIGHBOR_H
