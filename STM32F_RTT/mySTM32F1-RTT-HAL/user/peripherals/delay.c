#include "delay.h"
#include <rtthread.h>

void delay_us(int us)
{
		rt_uint32_t delta;
	/* 获得延时经过的tick数 */
	us = us * (SysTick->LOAD/(1000000/RT_TICK_PER_SECOND));
	/* 获得当前时间 */
	delta = SysTick->VAL;
	/* 循环获得当前时间，直到达到指定的时间后退出循环 */
	while (delta - SysTick->VAL< us);
}

void delay_ms(u16 nms)
{
	//rt_thread_delay(nms/10);
	drv_udelay(nms*1000);
}

void drv_udelay(uint32_t us)
{
    int i = (SystemCoreClock / 4000000 * us);
    while (i)
    {
        i--;
    }
}
