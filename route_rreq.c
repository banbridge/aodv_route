//
// Created by ByteDance on 2022/7/27.
//

#ifdef NS_PORT
#include "ns-2/aodv-uu.h"
#else
#include <netinet/in.h>

#include "route_rreq.h"
#include "route_rrep.h"
#include "route_routing_table.h"
#include "route_timeout.h"
#include "route_timer_queue.h"
#include "route_socket.h"
#include "route_params.h"
#include "route_seek_list.h"
#include "route_defs.h"
#include "route_debug.h"

#include "route_locality.h"
#endif

