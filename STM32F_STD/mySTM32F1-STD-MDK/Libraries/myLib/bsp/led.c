#include "led.h"
#include "stdio.h"	

void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;				
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOD, &GPIO_InitStructure);					 
 GPIO_ResetBits(GPIOD,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);						 

}
 
void rgb_set(char* Value)
{
	switch(Value[0])
	{
		case 'r':	RGB_R=1;RGB_G=0;RGB_B=0;
			break;
		case 'g':	RGB_R=0;RGB_G=1;RGB_B=0;
			break;
		case 'b':	RGB_R=0;RGB_G=0;RGB_B=1;
			break;
		default:	RGB_R=0;RGB_G=0;RGB_B=0;
			break;		
	}
	printf("<ok>");
}
