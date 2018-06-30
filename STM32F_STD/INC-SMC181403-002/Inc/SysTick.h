
/**
  \file SysTick.h

  \version 1.00

  \brief System tick timer

  This file contains the definitions of the tick timer functions.
*/

#ifndef __SYSTICK_H
#define __SYSTICK_H

void InitSysTick(void);
UINT GetSysTimer(void);

void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif

