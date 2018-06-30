#include "delay.h"
#include "stm32f10x.h"
#include <rtthread.h>

void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;							//LOAD的值	    	 
	ticks=nus*SystemCoreClock/1000000; 		//需要的节拍数
  //ticks=nus*SystemCoreClock/8000000;			//SYSTICK_CLKSOURCE_HCLK_DIV8	64/8=8
	told=SysTick->VAL;        						//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;			//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;							//时间超过/等于要延迟的时间,则退出.
		}  
	};						    
}

//延时nms
void delay_ms(u16 nms)
{	
	rt_thread_delay(nms/10);
	//delay_us((u32)(nms*1000));					
}

void rt_hw_us_delay(int us)
{
	rt_uint32_t delta;
	/* 获得延时经过的tick数 */
	us = us * (SysTick->LOAD/(1000000/RT_TICK_PER_SECOND));
	/* 获得当前时间 */
	delta = SysTick->VAL;
	/* 循环获得当前时间，直到达到指定的时间后退出循环 */
	while (delta - SysTick->VAL< us);
}
