#ifndef __DELAY_H
#define __DELAY_H 

#include "sys.h"

void rt_hw_us_delay(int us);
void rt_hw_ms_delay(u16 nms);

void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif
