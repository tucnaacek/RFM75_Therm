#ifndef MEM_24LC04B_H
#define MEM_24LC04B_H

#include "stdint.h"
#include "stdint-gcc.h"
#include "caliper.h"

//#define 	ACC_ADDRESS_RD	0x1D
//#define 	ACC_ADDRESS_WR	0x1C

#define		MEM_24LC04B_RADDR	  0xa1	//0x1D
#define		MEM_24LC04B_WADDR	  0xa0	//0x1C
#define   LOW_BLOCK(n) (n & 0xfd)   //clear B0
#define   HI_BLOCK(n) (n | 0x02)    //set B0


#endif // 24LC04B_H_INCLUDED
