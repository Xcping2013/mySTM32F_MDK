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
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	SysTick->LOAD=9000;
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   						//����SYSTICK�ж�
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   						//����SYSTICK    

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
	u32 reload=SysTick->LOAD;					//LOAD��ֵ	    	 
	ticks=nus*9; 											//��Ҫ�Ľ�����	  		 
	tcnt=0;

	told=SysTick->VAL;        					//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;		//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;				//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};						    
}
void delay_ms(u16 nms)
{	
	delay_us((u32)(nms*1000));					//��ͨ��ʽ��ʱ  
}

