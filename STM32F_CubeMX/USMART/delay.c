#include "delay.h"
#include "core_cm0.h"
#include "usart.h"

#define DELAY_DEBUG

#ifdef  DELAY_DEBUG

#define DELAY_TRACE         printf
#else
#define DELAY_TRACE(...)    
#endif

void delay_us(uint32_t nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;			//LOAD的值	    	 
	ticks=nus*48; 								//需要的节拍数 
	told=SysTick->VAL;        		//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};
	DELAY_TRACE("<ok>");
}

void delay_ms(uint32_t nms)
{
	HAL_Delay(nms);
	DELAY_TRACE("<ok>");
}






































