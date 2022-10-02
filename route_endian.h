//
// Created by ByteDance on 2022/8/7.
//

#ifndef AODV_ROUTE_ENDIAN_SELL_H
#define AODV_ROUTE_ENDIAN_SELL_H



#define LITTLE_ENDIAN 0x41424344UL
#define BIG_ENDIAN    0x44434241UL
#define PDP_ENDIAN    0x42414443UL
#define ENDIAN_ORDER  ('ABCD')

#if ENDIAN_ORDER == LITTLE_ENDIAN
#define __LITTLE_ENDIAN 1234
#elif ENDIAN_ORDER == BIG_ENDIAN
#define __BIG_ENDIAN    4321
#elif ENDIAN_ORDER==PDP_ENDIAN
#error "jeez, machine is PDP!"
#else
#error "What kind of hardware is this?!"
#endif

int litend(void) {
    int i = 0;
    ((char *) (&i))[0] = 1;
    return (i == 1);
}

int bigend(void) {
    return !litend();
}


#endif //AODV_ROUTE_ENDIAN_SELL_H