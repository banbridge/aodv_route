//
// Created by ByteDance on 2022/7/27.
//

#ifndef AODV_ROUTE_AODV_NEIGHBOR_H
#define AODV_ROUTE_AODV_NEIGHBOR_H

#include "route_defs.h"
#include "route_routing_table.h"



void NeighborAdd(AODV_msg * aodv_msg, struct in_addr source,
                  unsigned int ifindex);
void NeighborLinkBreak(RtTableT * rt);



#endif //AODV_ROUTE_AODV_NEIGHBOR_H
