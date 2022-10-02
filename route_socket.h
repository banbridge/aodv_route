//
// Created by ByteDance on 2022/7/27.
//

#ifndef AODV_ROUTE_AODV_SOCKET_H
#define AODV_ROUTE_AODV_SOCKET_H



#include <netinet/ip.h>

#include "route_defs.h"
#include "route_rerr.h"
#include "route_params.h"

#define IPHDR_SIZE sizeof(struct iphdr)

/* Set a maximun size for AODV msgs. The RERR is the potentially
   largest message, depending on how many unreachable destinations
   that are included. Lets limit them to 100 */
#define AODV_MSG_MAX_SIZE RERR_SIZE + 100 * RERR_UDEST_SIZE
#define RECV_BUF_SIZE AODV_MSG_MAX_SIZE
#define SEND_BUF_SIZE RECV_BUF_SIZE


struct timeval rreq_ratel[RREQ_RATELIMIT - 1], rerr_ratel[RERR_RATELIMIT - 1];
int num_rreq;
int num_rerr;

void AodvSocketInit();
void AodvSocketSend(AODV_msg * aodv_msg, struct in_addr dst, int len,
                      u_int8_t ttl, struct dev_info *dev);
AODV_msg *AodvSocketNewMsg();
AODV_msg *AodvSocketQueueMsg(AODV_msg * aodv_msg, int size);
void AodvSocketCleanup(void);
void AodvSocketProcessPacket(AODV_msg * aodv_msg, int len,
                                struct in_addr src, struct in_addr dst, int ttl,
                                unsigned int ifindex);
#define CMSG_NXTHDR_FIX(mhdr, cmsg) cmsg_nxthdr_fix((mhdr), (cmsg))
struct cmsghdr *cmsg_nxthdr_fix(struct msghdr *__msg, struct cmsghdr *__cmsg);




#endif //AODV_ROUTE_AODV_SOCKET_H
