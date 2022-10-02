//
// Created by ByteDance on 2022/7/25.
//

#ifndef AODV_ROUTE_PARAMS_H
#define AODV_ROUTE_PARAMS_H

#include "route_defs.h"

#define K                       5

/* Dynamic configuration values. Default numbers are for HELLO messages. */
//ACTIVE_ROUTE_TIMEOUT：一个路由的lifetime，可动态变化
#define ACTIVE_ROUTE_TIMEOUT active_route_timeout
//RREQ消息的初始TTL（time_to_live，可以理解为生存时间）值，它等于 ttl_start，这是一个动态变化的值
#define TTL_START ttl_start
//当一个路由项失效之后，再过这个时间才把它删除，可动态变化
#define DELETE_PERIOD delete_period

/* Settings for Link Layer Feedback */
#define ACTIVE_ROUTE_TIMEOUT_LLF    10000
#define TTL_START_LLF               1
#define DELETE_PERIOD_LLF           ACTIVE_ROUTE_TIMEOUT_LLF

/* Settings for HELLO messages */
#define ACTIVE_ROUTE_TIMEOUT_HELLO  3000
#define TTL_START_HELLO             2
#define DELETE_PERIOD_HELLO         K * max(ACTIVE_ROUTE_TIMEOUT_HELLO, ALLOWED_HELLO_LOSS * HELLO_INTERVAL)

/* Non runtime modifiable settings */
//当丢失了超过这个数目的hello消息之后，认为到那个节点的连接中断
#define ALLOWED_HELLO_LOSS      2
/* If expanding ring search is used, BLACKLIST_TIMEOUT should be?: */
//进入RREQ黑名单后在黑名单里面待的时间
#define BLACKLIST_TIMEOUT       RREQ_RETRIES * NET_TRAVERSAL_TIME + (TTL_THRESHOLD - TTL_START)/TTL_INCREMENT + 1 + RREQ_RETRIES
//广播hello消息的时间间隔
#define HELLO_INTERVAL          1000
//在路径修复过程中用来计算RREQ的TTL
#define LOCAL_ADD_TTL           2
//进行路径修复的最大跳数（也就是如果到目标结点的跳数超过这个，那么不进行路径修复）
#define MAX_REPAIR_TTL          3 * NET_DIAMETER / 10
//节点发送RREP消息时，给RREP消息设置的lifetime就是这个值，也就是RREP消息的生存时间
#define MY_ROUTE_TIMEOUT        2 * ACTIVE_ROUTE_TIMEOUT
//网络直径，也就是无线网络系统中两个节点间可能的最大跳数，它是RREQ的TTL的上界
#define NET_DIAMETER            35
//网络横跨时间
#define NET_TRAVERSAL_TIME      2 * NODE_TRAVERSAL_TIME * NET_DIAMETER
#define NEXT_HOP_WAIT           NODE_TRAVERSAL_TIME + 10
//节点横跨时间，可以理解为发包的时候，一跳所需要的时间
#define NODE_TRAVERSAL_TIME     40
//当一个节点广播RREQ消息之前，它会把RREQ的ID和源IP缓存这么长的时间，
// 这样当它收到邻居的RREQ时（实际上本来是自己发的）不会做出回应；
// 当一个节点收到RREQ时，它也会检查在过去的这么长时间内是否收到过同样的ID和源IP，如果有则丢弃这个RREQ
#define PATH_DISCOVERY_TIME     2 * NET_TRAVERSAL_TIME
//每秒发送RERR消息数目的限制
#define RERR_RATELIMIT          10
//节点进行路径搜索时用的方法是扩展环搜索（expanding ring search），
// 一开始给RREQ消息的TTL设置为TTL_START，如果在RING_TRAVERSAL_TIME内没
// 有受到RREP则认为搜索失败，然后再次广播RREQ，只不过把TTL在上一次的基础上
// 加上TTL_INCREMENT，然后再次搜索，直到TTL达到阈值TTL_THRESHOLD，之后
// 的每次搜索TTL都是NET_DIAMETER，等待RREP时间都是RING_TRAVERSAL_TIME
#define RING_TRAVERSAL_TIME     2 * NODE_TRAVERSAL_TIME * (TTL_VALUE + TIMEOUT_BUFFER)
//每次寻路时发送RREQ数目上限
#define RREQ_RETRIES            2
//每秒发送RREQ的数目上限
#define RREQ_RATELIMIT          10
#define TIMEOUT_BUFFER          2
#define TTL_INCREMENT           2
#define TTL_THRESHOLD           7

#ifndef NS_PORT
/* Dynamic configuration values */
extern int active_route_timeout;
extern int ttl_start;
extern int delete_period;
#endif


#endif //AODV_ROUTE_PARAMS_H
