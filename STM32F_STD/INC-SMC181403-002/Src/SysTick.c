/**
  \file SysTick.c

  \version 1.00

  \brief 1ms system tick timer functions

  This file provides all functions needed for easy
  access to the TMC262 stepper motor driver IC.
*/

#include "main.h"

static volatile UINT SysTickTimer;
/***************************************************//**
  \fn SysTick_Handler(void)
  \brief System timer interrupt handler

  The system timer interrupt handler counts up the
  1ms counter.
*******************************************************/
void SysTick_Handler(void)
{
	SysTickTimer++;
}
/***************************************************//**
  \fn InitSysTick(void)
  \brief Initalize system timer

  Initalize the tick timer for generating an interrupt
  every 1ms.
*******************************************************/
void InitSysTick(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	SysTick->LOAD=9000;
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   						//开启SYSTICK中断
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   						//开启SYSTICK    

}
/***************************************************//**
  \fn GetSysTimer(void)
  \brief Read the system timer
  \return System timer (1ms)

  This function returns the actual value of the 1ms
  system timer.
*******************************************************/
UINT GetSysTimer(void)
{
  return SysTickTimer;
}
//
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;					//LOAD的值	    	 
	ticks=nus*9; 											//需要的节拍数	  		 
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

