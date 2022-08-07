cmake_minimum_required(VERSION 3.22)
project(aodv_route C)

set(CMAKE_C_STANDARD 99)

add_executable(aodv_route main.c defs.h list.h list.c params.h timer_queue.h timer_queue.c debug.h debug.c seek_list.c seek_list.h aodv_timeout.c aodv_timeout.h aodv_hello.c aodv_hello.h aodv_neighbor.c aodv_neighbor.h aodv_rerr.c aodv_rerr.h aodv_rrep.c aodv_rrep.h aodv_rreq.c aodv_rreq.h aodv_socket.c aodv_socket.h routing_table.c routing_table.h endian_sell.h)
