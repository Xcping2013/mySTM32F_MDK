/**						
  \Project		IN3-SMC-3
	\file 			IO.h
  \author 		Xcping2016
  \version 		1.00

  \brief 			I/O functions
  This file contains definitions for using the I/O functions.
*/
#ifndef __OUTPUT_H
#define __OUTPUT_H

#include "sys.h"

#define OUTPUT1		PAout(15)
#define OUTPUT2		PCout(10)
#define OUTPUT3		PCout(11)
#define OUTPUT4		PCout(12)
#define OUTPUT5		PDout(0)
#define OUTPUT6		PDout(1)
#define OUTPUT7		PDout(2)
#define OUTPUT8		PDout(3)

#define OUTPUT9		PBout(14)
#define OUTPUT10	PDout(10)
#define OUTPUT11	PDout(13)
#define OUTPUT12	PDout(11)
#define OUTPUT13	PEout(14)
#define OUTPUT14	PEout(15)

void 		output_init(void);
void 		output_write(uint8_t channel, uint8_t value);

#endif
