#ifndef __DELAY_H
#define __DELAY_H 

#include "sys.h"

void delay_us(int us);
void delay_ms(u16 nms);

void drv_udelay(uint32_t us);

#endif
