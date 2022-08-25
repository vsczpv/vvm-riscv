#include <ministd.h>

 /*
 * Taken from GLIBC, Copyright (C) 2009-2022 Free Software Foundation, Inc.
 * Check mirror below for full licensing disclosure.
 * https://github.com/gcc-mirror/gcc/blob/master/libgcc/config/epiphany/mulsi3.c
 *
 * vvm-riscv does not support hardware multiplication, so a software stub has to be provided.
 *
 * GCC automagically replaces the * (multiplication) operator by the __mulsi3 function
 *
 */
unsigned int
__mulsi3 (unsigned int a, unsigned int b)
{
  unsigned int r = 0;

  while (a)
    {
      if (a & 1)
	r += b;
      a >>= 1;
      b <<= 1;
    }
  return r;
}

/* Taken from https://stackoverflow.com/questions/34457575/integer-division-algorithm-analysis */
unsigned int __divsi3(unsigned int num, unsigned int den)
{

	if (den == 0) { putstring("DISIVION BY ZERO EXCEPTION"); exit(); }
    unsigned place=1;
    unsigned ret=0;
    while((num>>1) >= den) //overflow-safe check
    {
        place<<=1;
        den<<=1;
    }
    for( ;place>0; place>>=1,den>>=1)
    {
       if (num>=den)
       {
           num-=den;
           ret+=place;
       }
    }
    return ret;
}

unsigned int __modsi3(unsigned int num, unsigned int den)
{
	if (den == 0) { putstring("DISIVION BY ZERO EXCEPTION"); exit(); }

	for (;;)
	{
		if (num >= den)
		{
			num -= den;
		} else return num;
	}

}
