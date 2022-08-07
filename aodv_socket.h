//
// Created by ByteDance on 2022/7/27.
//

#ifndef AODV_ROUTE_AODV_SOCKET_H
#define AODV_ROUTE_AODV_SOCKET_H


#ifndef NS_NO_GLOBALS
#include <netinet/ip.h>

#include "defs.h"
#include "aodv_rerr.h"
#include "params.h"

#define IPHDR_SIZE sizeof(struct iphdr)

/* Set a maximun size for AODV msgs. The RERR is the potentially
   largest message, depending on how many unreachable destinations
   that are included. Lets limit them to 100 */
#define AODV_MSG_MAX_SIZE RERR_SIZE + 100 * RERR_UDEST_SIZE
#define RECV_BUF_SIZE AODV_MSG_MAX_SIZE
#define SEND_BUF_SIZE RECV_BUF_SIZE
#endif				/* NS_NO_GLOBALS */

#ifndef NS_NO_DECLARATIONS

struct timeval rreq_ratel[RREQ_RATELIMIT - 1], rerr_ratel[RERR_RATELIMIT - 1];
int num_rreq;
int num_rerr;

void aodv_socket_init();
void aodv_socket_send(AODV_msg * aodv_msg, struct in_addr dst, int len,
                      u_int8_t ttl, struct dev_info *dev);
AODV_msg *aodv_socket_new_msg();
AODV_msg *aodv_socket_queue_msg(AODV_msg * aodv_msg, int size);
void aodv_socket_cleanup(void);
void aodv_socket_process_packet(AODV_msg * aodv_msg, int len,
                                struct in_addr src, struct in_addr dst, int ttl,
                                unsigned int ifindex);
#define CMSG_NXTHDR_FIX(mhdr, cmsg) cmsg_nxthdr_fix((mhdr), (cmsg))
struct cmsghdr *cmsg_nxthdr_fix(struct msghdr *__msg, struct cmsghdr *__cmsg);

#ifdef NS_PORT
void recvAODVUUPacket(Packet * p);
#endif				/* NS_PORT */

#endif				/* NS_NO_DECLARATIONS */


#endif //AODV_ROUTE_AODV_SOCKET_H
