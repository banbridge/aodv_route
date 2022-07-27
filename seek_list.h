/*
 * @Author: 高逸博 gaoyibo.09@bytedance.com
 * @Date: 2022-07-27 08:46:57
 * @LastEditors: 高逸博 gaoyibo.09@bytedance.com
 * @LastEditTime: 2022-07-27 08:56:15
 * @FilePath: /aodv-route/seek_list.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
//
// Created by ByteDance on 2022/7/27.
//

/**
 * @brief 
 * 这个文件是用来实现 seek list 的，seeklist记录的是寻路的队列（有点儿神奇），实现了一个寻路请求的结构体 seek_list_t 
 * 以及寻路队列链表头 seedhead，该队列支持插入、删除、（通过 dest_addr）查找的操作。其中每个 entry（seek_list_t）的结构体包含如下内容
 * l：链表域
    dest_addr：寻路目的节点的地址
    dest_seqno：目的节点的序列号
    ipd：一个 ip_data 类型的数据，记录一个地址，当寻路失败时向这个地址发送目的主机不可达的ICMP报文
    flags：一个用来重发送RREQ的标识
    reqs：已经发送RREQ的数目
    ttl：发送RREQ时使用的TTL值
    seek_timer：寻路使用的计时器，绑定的处理函数是 route_discovery_timeout，这个函数的功能是：如果 reqs 小于 RREQ_RETRIES（定义与 params.h），那么 reqs自增，ttl增加（增加方式也已经说过），然后重新设置 seek_timer，时间为 RING_TRAVERSAL_TIME，然后更新路由表中相应目的节点表项的timeout（表示，啊，我要重新发送RREQ了，所以你还得再等 2 * NET_TRAVERSAL_TIME 的时间才能从路由表中删去），然后发送RREQ；否则的话，表示已经达到了一次寻路所限制的最大的发送RREQ数目限制，那么就认为寻路失败，寻路失败首先调用了一个 nl 相关的函数发送一些寻路失败信息，然后移出当前seek表项，然后从路由表找到目的节点路由信息（可能已经失效但是还没有移除），如果该表项的flags标识了RT_REPAIR，那么调用local_repair_timeout（表示当前发送RREQ是为了路由修复，然后修复失败了，它的作用主要是广播RERR信息，然后生成一个删除路由的timer，绑定 route_delete_timeout函数，然后在DELETE_PERIOD之后删除该路由）

 */
#ifndef AODV_ROUTE_SEEK_LIST_H
#define AODV_ROUTE_SEEK_LIST_H

#ifndef NS_NO_GLOBALS
#include "defs.h"
#include "timer_queue.h"
#include "list.h"

#define IP_DATA_DATA_MAX_LEN 60+8 /* Max IP header + 64 bits of data */

struct ip_data{
    char data[IP_DATA_DATA_MAX_LEN];
    int len;
};

/* This is a list of nodes that route discovery are performed for */
typedef struct seek_list{
    list_t l;
    struct in_addr dest_addr;
    u_int32_t dest_seqno;
    struct ip_data *ipd;
    u_int8_t flags;		/* The flags we are using for resending the RREQ */
    int reqs;
    int ttl;
    struct timer seek_timer;
}seek_list_t;

#endif /* NS_NO_GLOBALS */

#ifndef NS_NO_DECLARATIONS
seek_list_t *seek_list_insert(struct in_addr dest_addr, u_int32_t dest_seqno,
			      int ttl, u_int8_t flags, struct ip_data *ipd);
int seek_list_remove(seek_list_t * entry);
seek_list_t *seek_list_find(struct in_addr dest_addr);

#ifdef NS_PORT
#ifdef SEEK_LIST_DEBUG
void seek_list_print();
#endif
#endif				/* NS_PORT */

#endif				/* NS_NO_DECLARATIONS */

#endif //AODV_ROUTE_SEEK_LIST_H
