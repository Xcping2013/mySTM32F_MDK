#include "delay.h"

//static u8  fac_us=0;								   
//static u16 fac_ms=0;							
	
static volatile u32 SysTickTimer;

void SysTick_Handler(void)
{	
	SysTickTimer++;
}

u32 GetSysTimer(void)
{
  return SysTickTimer;
}

//void delay_init()
//{
//	fac_us=SystemCoreClock/1000000;				
//	fac_ms=(u16)fac_us*1000;		
//	SysTick->LOAD=fac_ms;
//	SysTick->CTRL=SysTick_CTRL_CLKSOURCE_Msk;	
//	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   						//开启SYSTICK中断
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; 	
//}								    
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
//
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;						//LOAD的值	    	 
	ticks=nus*48; 											//需要的节拍数	  		 
	tcnt=0;

	told=SysTick->VAL;        					//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;		//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;				//时间超过/等于要延迟的时间,则退出.
		}  
	};						    
}
void delay_ms(u16 nms)
{	
	delay_us((u32)(nms*1000));					//普通方式延时  
}





































