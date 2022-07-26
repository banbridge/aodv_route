cmake_minimum_required(VERSION 3.22)
project(aodv_route C)

set(CMAKE_C_STANDARD 99)

add_executable(aodv_route main.c defs.h list.h list.c params.h timer_queue.h timer_queue.c debug.h debug.c)
