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
	u32 reload=SysTick->LOAD;			//LOAD��ֵ	    	 
	ticks=nus*48; 								//��Ҫ�Ľ����� 
	told=SysTick->VAL;        		//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
	DELAY_TRACE("<ok>");
}

void delay_ms(uint32_t nms)
{
	HAL_Delay(nms);
	DELAY_TRACE("<ok>");
}






































