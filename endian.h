//
// Created by ByteDance on 2022/7/27.
//

#ifndef AODV_ROUTE_ENDIAN_H
#define AODV_ROUTE_ENDIAN_H

#include <stdio.h>

int litend(void)
{
    int i = 0;
    ((char *) (&i))[0] = 1;
    return (i == 1);
}

int bigend(void)
{
    return !litend();
}

int main(int argc, char **argv)
{
    printf("#ifndef ENDIAN_H\n");
    printf("#define ENDIAN_H\n");
    printf("#define __LITTLE_ENDIAN 1234\n");
    printf("#define __BIG_ENDIAN    4321\n");
    printf("#define __BYTE_ORDER __%s_ENDIAN\n", litend()? "LITTLE" : "BIG");
    printf("#endif\n");
    return 0;
}

#endif //AODV_ROUTE_ENDIAN_H
