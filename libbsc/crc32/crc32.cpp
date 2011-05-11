/*-----------------------------------------------------------*/
/* Block Sorting, Lossless Data Compression Library.         */
/* CRC32 (cyclic redundancy check) functions                 */
/*-----------------------------------------------------------*/

/*--

This file is a part of bsc and/or libbsc, a program and a library for
lossless, block-sorting data compression.

Copyright (c) 2009-2011 Ilya Grebnov <ilya.grebnov@libbsc.com>

The bsc and libbsc is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The bsc and libbsc is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the bsc and libbsc. If not, see http://www.gnu.org/licenses/.

Please see the files COPYING and COPYING.LIB for full copyright information.

See also the bsc and libbsc web site:
  http://libbsc.com/ for more information.

--*/

#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include "crc32.h"

#include "../common/common.h"
#include "../libbsc.h"

#define BASE 65521UL
#define NMAX 5552

#define DO1(buf,i)  {adler += (buf)[i]; sum2 += adler;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);
#define MOD(a)      a %= BASE

unsigned int bsc_crc32(const unsigned char * T, int n)
{
	unsigned int adler = 1;
    unsigned int sum2  = 0;

    while (n >= NMAX) 
    {
        for (int i = 0; i < NMAX / 16; ++i)
        {
            DO16(T); T += 16;
        }
        MOD(adler); MOD(sum2); n -= NMAX;
    }

    while (n >= 16) 
    {
        DO16(T); T += 16; n -= 16; 
    }

    while (n > 0)
    {
        DO1(T, 0); T += 1; n -= 1; 
    }
    
    MOD(adler); MOD(sum2);

    return adler | (sum2 << 16);
}

/*-----------------------------------------------------------*/
/* End                                             crc32.cpp */
/*-----------------------------------------------------------*/
