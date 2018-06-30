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
//	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   						//����SYSTICK�ж�
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; 	
//}								    
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
//
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;						//LOAD��ֵ	    	 
	ticks=nus*48; 											//��Ҫ�Ľ�����	  		 
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





































