/*
Copyright 2018 Embedded Microprocessor Benchmark Consortium (EEMBC)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <math.h>
#define CVTBUFSIZE 80
static char CVTBUF[CVTBUFSIZE];

#define __HI(x) *(1+(int*)&x)
#define __LO(x) *(int*)&x
#define __HIp(x) *(1+(int*)x)
#define __LOp(x) *(int*)x

static double one = 1.0;

double modf(double x, double *iptr)
{
    int i0,i1,j0;
    unsigned i;
    i0 =  __HI(x);      /* high x */
    i1 =  __LO(x);      /* low  x */
    j0 = ((i0>>20)&0x7ff)-0x3ff;    /* exponent of x */
    if(j0<20) {         /* integer part in high x */
        if(j0<0) {          /* |x|<1 */
        __HIp(iptr) = i0&0x80000000;
        __LOp(iptr) = 0;        /* *iptr = +-0 */
        return x;
        } else {
        i = (0x000fffff)>>j0;
        if(((i0&i)|i1)==0) {        /* x is integral */
            *iptr = x;
            __HI(x) &= 0x80000000;
            __LO(x)  = 0;   /* return +-0 */
            return x;
        } else {
            __HIp(iptr) = i0&(~i);
            __LOp(iptr) = 0;
            return x - *iptr;
        }
        }
    } else if (j0>51) {     /* no fraction part */
        *iptr = x*one;
        __HI(x) &= 0x80000000;
        __LO(x)  = 0;   /* return +-0 */
        return x;
    } else {            /* fraction part in low x */
        i = ((unsigned)(0xffffffff))>>(j0-20);
        if((i1&i)==0) {         /* x is integral */
        *iptr = x;
        __HI(x) &= 0x80000000;
        __LO(x)  = 0;   /* return +-0 */
        return x;
        } else {
        __HIp(iptr) = i0;
        __LOp(iptr) = i1&(~i);
        return x - *iptr;
        }
    }
}


static char *
cvt(double arg, int ndigits, int *decpt, int *sign, char *buf, int eflag)
{
    int    r2;
    double fi, fj;
    char * p, *p1;

    if (ndigits < 0)
        ndigits = 0;
    if (ndigits >= CVTBUFSIZE - 1)
        ndigits = CVTBUFSIZE - 2;
    r2    = 0;
    *sign = 0;
    p     = &buf[0];
    if (arg < 0)
    {
        *sign = 1;
        arg   = -arg;
    }
    arg = modf(arg, &fi);
    p1  = &buf[CVTBUFSIZE];

    if (fi != 0)
    {
        p1 = &buf[CVTBUFSIZE];
        while (fi != 0)
        {
            fj    = modf(fi / 10, &fi);
            *--p1 = (int)((fj + .03) * 10) + '0';
            r2++;
        }
        while (p1 < &buf[CVTBUFSIZE])
            *p++ = *p1++;
    }
    else if (arg > 0)
    {
        while ((fj = arg * 10) < 1)
        {
            arg = fj;
            r2--;
        }
    }
    p1 = &buf[ndigits];
    if (eflag == 0)
        p1 += r2;
    *decpt = r2;
    if (p1 < &buf[0])
    {
        buf[0] = '\0';
        return buf;
    }
    while (p <= p1 && p < &buf[CVTBUFSIZE])
    {
        arg *= 10;
        arg  = modf(arg, &fj);
        *p++ = (int)fj + '0';
    }
    if (p1 >= &buf[CVTBUFSIZE])
    {
        buf[CVTBUFSIZE - 1] = '\0';
        return buf;
    }
    p = p1;
    *p1 += 5;
    while (*p1 > '9')
    {
        *p1 = '0';
        if (p1 > buf)
            ++*--p1;
        else
        {
            *p1 = '1';
            (*decpt)++;
            if (eflag == 0)
            {
                if (p > buf)
                    *p = '0';
                p++;
            }
        }
    }
    *p = '\0';
    return buf;
}

char *
ecvt(double arg, int ndigits, int *decpt, int *sign)
{
    return cvt(arg, ndigits, decpt, sign, CVTBUF, 1);
}

char *
ecvtbuf(double arg, int ndigits, int *decpt, int *sign, char *buf)
{
    return cvt(arg, ndigits, decpt, sign, buf, 1);
}

char *
fcvt(double arg, int ndigits, int *decpt, int *sign)
{
    return cvt(arg, ndigits, decpt, sign, CVTBUF, 0);
}

char *
fcvtbuf(double arg, int ndigits, int *decpt, int *sign, char *buf)
{
    return cvt(arg, ndigits, decpt, sign, buf, 0);
}
