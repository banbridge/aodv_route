cmake_minimum_required(VERSION 3.21.1)
project(aodv_route C)

set(CMAKE_C_STANDARD 99)

add_executable(aodv_route main.c route_endian.h route_defs.h route_list.h route_list.c route_params.h route_timer_queue.h route_timer_queue.c route_debug.h route_debug.c route_seek_list.c route_seek_list.h route_timeout.c route_timeout.h route_hello.c route_hello.h route_neighbor.c route_neighbor.h route_rerr.c route_rerr.h route_rrep.c route_rrep.h route_rreq.c route_rreq.h route_socket.c route_socket.h route_routing_table.c route_routing_table.h route_endian.h route_locality.c route_locality.h)
