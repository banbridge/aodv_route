//
// Created by ByteDance on 2022/8/7.
//

#ifndef AODV_ROUTE_LOCALITY_H
#define AODV_ROUTE_LOCALITY_H

#ifndef NS_NO_GLOBALS

enum {
    HOST_ADHOC,
    HOST_INET,
    HOST_UNKNOWN
};
#endif				/* NS_NO_GLOBALS */

#ifndef NS_NO_DECLARATIONS
int locality(struct in_addr dest, unsigned int ifindex);
#endif				/* NS_NO_DECLARATIONS */

#endif //AODV_ROUTE_LOCALITY_H
