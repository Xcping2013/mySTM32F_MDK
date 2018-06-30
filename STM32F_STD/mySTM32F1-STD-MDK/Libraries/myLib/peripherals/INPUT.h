
#ifndef __INPUT_H
#define __INPUT_H

#include "sys.h"

#define INPUT1	PBin(5)
#define INPUT2	PBin(6)
#define INPUT3	PBin(7)
#define INPUT4	PBin(8)
#define INPUT5	PBin(9)

#define INPUT6	PEin(0)
#define INPUT7	PEin(1)
#define INPUT8	PEin(2)
#define INPUT9	PEin(3)
#define INPUT10	PEin(4)
#define INPUT11	PEin(5)
#define INPUT12	PEin(6)

#define INPUT13	PBin(1)
#define INPUT14	PDin(8)
#define INPUT15	PDin(12)
#define INPUT16	PDin(9)
#define INPUT17	PBin(15)
#define INPUT18	PBin(13)

void 	input_init(void);
void 	input_read(uint8_t channel);
void 	input_reads(void);

#endif
