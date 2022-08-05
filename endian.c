//
// Created by ByteDance on 2022/8/5.
//

/* It solves the problem of non-existent <endian.h> on some systems by
  generating it.

  (Compile, run, and redirect the output to endian.h.)
*/

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
