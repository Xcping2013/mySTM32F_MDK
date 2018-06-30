#include "delay.h"
#include "stm32f10x.h"
#include <rtthread.h>

void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;							//LOAD��ֵ	    	 
	ticks=nus*SystemCoreClock/1000000; 		//��Ҫ�Ľ�����
  //ticks=nus*SystemCoreClock/8000000;			//SYSTICK_CLKSOURCE_HCLK_DIV8	64/8=8
	told=SysTick->VAL;        						//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;			//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;							//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};						    
}

//��ʱnms
void delay_ms(u16 nms)
{	
	rt_thread_delay(nms/10);
	//delay_us((u32)(nms*1000));					
}

void rt_hw_us_delay(int us)
{
	rt_uint32_t delta;
	/* �����ʱ������tick�� */
	us = us * (SysTick->LOAD/(1000000/RT_TICK_PER_SECOND));
	/* ��õ�ǰʱ�� */
	delta = SysTick->VAL;
	/* ѭ����õ�ǰʱ�䣬ֱ���ﵽָ����ʱ����˳�ѭ�� */
	while (delta - SysTick->VAL< us);
}
