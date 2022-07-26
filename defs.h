//
// Created by ByteDance on 2022/7/25.
//


/**
 * 定义了一些基本信息（比如aodvuu版本信息，说明文档名字等），定义了两个结构体：

dev_info：主要是当前机器可用的 network device（网络设备，可以理解为网卡） 的信息，
 包括状态enabled、socket、接口名字ifname、接口序号ifindex、ip地址、网络掩码netmask、广播地址broadcast；
host_info：当前结点的（最近一次使用的）序列号、上次广播时间bcast_time、上次发包时间fwd_time、RREQ序列号rreq_id、网络接口数目nif、网卡信息devs。
其中定义时间用的是 timeval 这个结构体，它是一个包含x秒y微妙（这两个域）的一个简单结构体；

定义地址用的是 in_addr 这个结构体，它是这样一个结构体：

struct in_addr {
        union {
                struct { u_char s_b1,s_b2,s_b3,s_b4; } S_un_b;
                struct { u_short s_w1,s_w2; } S_un_w;
                u_long S_addr;
        } S_un;

我也不知道到底是什么，反正可以用来表示 ip 地址。

除此之外，该文件还定义了如下一些东西：

MAX_NR_INTERFACE：最大网络接口数
dev_indices[MAX_NR_INTERFACE]：网卡信息的数组
ifindex、devindex、dev、name之间的各种等价类型转换
DEV_IFINDEX(ifindex)、DEV_NR(n)：获取 dev 信息（也就是dev的dev_info结构体）的比较方便的宏定义
AODV_BROADCAST：广播地址 ((in_addr_t) 0xFFFFFFFF)
AODV信息种类：AODV_HELLO(0), AODV_RREQ(1), AODV_RREP(2), AODV_RERR(3), AODV_RREP_ACK(4)
AODV_msg：这是一个信息类别，所有的RREQ、RREP等等消息在发送前都会转换为这个消息，收到这个消息时可以根据前面的类型再转换为对应的具体的消息类别
extension：不知道干嘛的，说是可以给消息附加邻居信息？
callback function：定义了一个带有一个int参数的函数指针 callback_func_t
 */

#ifndef AODV_ROUTE_DEFS_H
#define AODV_ROUTE_DEFS_H

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/time.h"
#include "sys/types.h"

#ifndef NS_PORT

#include "sys/signal.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "netinet/ip.h"
#include "sys/ioctl.h"

#endif /*NS_PORT*/

#include "syslog.h"
#include "errno.h"
#include "string.h"
#include "fcntl.h"

#ifndef NS_PORT

//#include "timer_queue.h"

#endif /*NS_PORT*/

#ifdef NS_PORT
#define NS_CLASS AODVUU::
#define NS_OUTSIDE_CLASS ::
#define NS_STATIC
#define NS_INLINE
/* NS_PORT: Using network device 0, with interface index 0. */
#define NS_DEV_NR 0
#define NS_IFINDEX NS_DEV_NR
#else
#define NS_CLASS
#define NS_OUTSIDE_CLASS
#define NS_STATIC static
#define NS_INLINE inline
#endif /* NS_PORT */

#define AODV_UU_VERSION "0.9"
#define DRAFT_VERSION "rfc3561"

#ifdef NS_PORT
/* NS_PORT: Log filename split into prefix and suffix. */
#define AODV_LOG_PATH_PREFIX "aodv-uu-"
#define AODV_RT_LOG_PATH_SUFFIX ".rtlog"
#define AODV_LOG_PATH_SUFFIX ".log"
#else
#define AODV_LOG_PATH "/var/log/aodvd.log"
#define AODV_RT_LOG_PATH "/var/log/aodvd.rtlog"
#endif                /* NS_PORT */

#define max(A, B) ( (A) > (B) ? (A):(B))

#define MINTTL 1        /* min TTL in the packets sent locally */

#define MAX_NR_INTERFACES 10
#define MAX_IFINDEX (MAX_NR_INTERFACES - 1)

#if !defined(IFNAMSIZ)
#define IFNAMSIZ 16
#endif /*!defined(IFNAMSIZ)*/


/* Data for a network device */
struct dev_info {
    int enabled;        /* 1 if struct is used, else 0 */
    int sock;            /* AODV socket associated with this device */
#ifdef CONFIG_GATEWAY
    int psock;			/* Socket to send buffered data packets. */
#endif /*CONFIG_GATEWAY*/
    unsigned int ifindex;
    char ifname[IFNAMSIZ];
    struct in_addr ipaddr;    /* The local IP address */
    struct in_addr netmask;    /* The netmask we use */
    struct in_addr broadcast;
};

struct host_info {
    u_int32_t seqno;        /* Sequence number */
    struct timeval bcast_time;    /* The time of the last broadcast msg sent */
    struct timeval fwd_time;    /* The time a data packet was last forwarded */
    u_int32_t rreq_id;        /* RREQ id */
    int nif;            /* Number of interfaces to broadcast on */
    struct dev_info devs[MAX_NR_INTERFACES + 1]; /* Add +1 for returning as "error" in ifindex2devindex. */
};


/*
  NS_PORT: TEMPORARY SOLUTION: Moved the two variables into the AODVUU class,
  and placed the function definition after the AODVUU class definition.

  (This is to avoid running several passes through defs.h during the source
  code extraction performed by the AODVUU class.)

  TODO: Find some smarter way to accomplish this.
*/
#ifndef NS_PORT
/* This will point to a struct containing information about the host */
struct host_info this_host;


/* Array of interface indexes */
unsigned int dev_indices[MAX_NR_INTERFACES];

/* Given a network interface index, return the index into the
   devs array, Necessary because ifindex is not always 0, 1,
   2... */
static inline unsigned int ifindex2devindex(unsigned int ifindex) {
    int i;

    for (i = 0; i < this_host.nif; i++)
        if (dev_indices[i] == ifindex)
            return i;

    return MAX_NR_INTERFACES;
}

static inline struct dev_info *devfromsock(int sock) {
    int i;

    for (i = 0; i < this_host.nif; i++) {
        if (this_host.devs[i].sock == sock)
            return &this_host.devs[i];
    }
    return NULL;
}

static inline int name2index(char *name) {
    int i;

    for (i = 0; i < this_host.nif; i++)
        if (strcmp(name, this_host.devs[i].ifname) == 0)
            return this_host.devs[i].ifindex;

    return -1;
}

#endif /*NS_PORT*/


/* Two macros to simplify retriving of a dev_info struct. Either using
   an ifindex or a device number (index into devs array). */
#define DEV_IFINDEX(ifindex) (this_host.devs[ifindex2devindex(ifindex)])
#define DEV_NR(n) (this_host.devs[n])

/* Broadcast address according to draft (255.255.255.255) */
#define AODV_BROADCAST ((in_addr_t) 0xFFFFFFFF)

#define AODV_PORT 654

/* AODV Message types */
#define AODV_HELLO    0        /* Really never used as a separate type... */
#define AODV_RREQ     1
#define AODV_RREP     2
#define AODV_RERR     3
#define AODV_RREP_ACK 4

/* A generic AODV packet header struct... */
#ifdef NS_PORT
struct AODV_msg {
#else
typedef struct {
#endif
    u_int8_t type;

/* NS_PORT: Additions for the AODVUU packet type in ns-2 */
#ifdef NS_PORT
    static int offset_;		// Required by PacketHeaderManager

    inline static int &offset() {
    return offset_;
    }
    inline static AODV_msg *access(const Packet * p) {
    return (AODV_msg *) p->access(offset_);
    }

    int size();
};

typedef AODV_msg hdr_aodvuu;	// Name convention for headers
#define HDR_AODVUU(p) ((hdr_aodvuu *) hdr_aodvuu::access(p))
#else
} AODV_msg;
#endif

/* AODV Extension types */
#define RREQ_EXT 1
#define RREP_EXT 1
#define RREP_HELLO_INTERVAL_EXT 2
#define RREP_HELLO_NEIGHBOR_SET_EXT 3
#define RREP_INET_DEST_EXT 4

/* An generic AODV extensions header */
typedef struct {
    u_int8_t type;
    u_int8_t length;
    /* Type specific data follows here */
} AODV_ext;

/* MACROS to access AODV extensions... */
#define AODV_EXT_HDR_SIZE sizeof(AODV_ext)
#define AODV_EXT_DATA(ext) ((char *)((char *)ext + AODV_EXT_HDR_SIZE))
#define AODV_EXT_NEXT(ext) ((AODV_ext *)((char *)ext + AODV_EXT_HDR_SIZE + ext->length))
#define AODV_EXT_SIZE(ext) (AODV_EXT_HDR_SIZE + ext->length)

#ifndef NS_PORT

/* The callback function */
typedef void (*callback_func_t)(int);

extern int attach_callback_func(int fd, callback_func_t func);

#endif /*NS_PORT*/

#endif /*AODV_ROUTE_DEFS_H*/
