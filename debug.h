//
// Created by ByteDance on 2022/7/26.
//

#ifndef AODV_ROUTE_DEBUG_H
#define AODV_ROUTE_DEBUG_H

#ifndef NS_PORT
extern int debug;
#endif

#ifndef NS_NO_DECLARATIONS
void log_init();
void log_cleanup();

char *packet_type(unsigned int type);
void alog(int type, int errnum, const char *function, char *format, ...);
void log_pkt_fields(AODV_msg * msg);
void print_rt_table(void *arg);
void log_rt_table_init();
char *ip_to_str(struct in_addr addr);

#ifdef NS_PORT
void write_to_log_file(char *msg, int len);
char *devs_ip_to_str();
char *rreq_flags_to_str(RREQ * rreq);
char *rrep_flags_to_str(RREP * rrep);
char *rt_flags_to_str(u_int16_t flags);
char *state_to_str(u_int8_t state);
#endif
#endif				/* NS_NO_DECLARATIONS */

#ifndef NS_NO_GLOBALS
#ifdef DEBUG
#undef DEBUG
#define DEBUG_OUTPUT
#define DEBUG(l, s, args...) alog(l, s, __FUNCTION__, ## args)
#else
#define DEBUG(l, s, args...)
#endif
#endif				/* NS_NO_GLOBALS */

#endif //AODV_ROUTE_DEBUG_H
