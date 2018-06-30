#include "stdio.h"	
#include "INPUT.h"
#include "OUTPUT.h"

#define INPUT_DEBUG

#ifdef INPUT_DEBUG

#define INPUT_TRACE         printf
#else
#define INPUT_TRACE(...)    
#endif

void input_init(void)
{
  // GPIO init
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	//IN1~5																											    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//IN6~12																										    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
//
void input_read(uint8_t channel)
{
	static uint8_t In9_14_Inited;
	uint8_t value;
	if(In9_14_Inited==0&&(channel>8))
	{
		GPIO_InitTypeDef  GPIO_InitStructure;
		In9_14_Inited=1;
 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	

		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		//INT13 IN17	IN18
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_15|GPIO_Pin_13;
		GPIO_Init(GPIOB, &GPIO_InitStructure);                                                 
		//INT14~16
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_12|GPIO_Pin_9;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	}
	if(0<channel&&channel<19)
	{
		switch(channel)
		{
			case 1: 	value=INPUT1 	? 0:1;			break;
			case 2: 	value= INPUT2 	? 0:1;		break;
			case 3: 	value= INPUT3 	? 0:1;		break;
			case 4: 	value= INPUT4 	? 0:1;		break;
			case 5: 	value= INPUT5 	? 0:1;		break;
			case 6: 	value= INPUT6 	? 0:1;    break;
			case 7: 	value= INPUT7 	? 0:1;    break;
			case 8: 	value= INPUT8 	? 0:1;    break;
			case 9: 	value= INPUT9 	? 0:1;    break;
			case 10: 	value= INPUT10	? 0:1;    break;
			case 11: 	value= INPUT11 	? 0:1;    break;
			case 12: 	value= INPUT12 	? 0:1;    break;
			case 13: 	value= INPUT13 	? 0:1;    break;
			case 14: 	value= INPUT14 	? 0:1;    break;
			case 15: 	value= INPUT15 	? 0:1;    break;
			case 16: 	value= INPUT16 	? 0:1;    break;
			case 17: 	value= INPUT17 	? 0:1;    break;
			case 18: 	value= INPUT18 	? 0:1;    break;
			default:  value= 255;								break;
		}	
		printf("<%d>",value);
	}
	else printf("channle must be 1~18");
}
//
void input_reads(void)
{
	u8 inputs[12];
	inputs[0]= INPUT1 		? '0':'1';
	inputs[1]= INPUT2 		? '0':'1';
	inputs[2]= INPUT3 		? '0':'1';
	inputs[3]= INPUT4 		? '0':'1';
	inputs[4]= INPUT5 		? '0':'1';
	inputs[5]= INPUT6 		? '0':'1';
	inputs[6]= INPUT7 		? '0':'1';
	inputs[7]= INPUT8 		? '0':'1';
	inputs[8]= INPUT9 		? '0':'1';
	inputs[9]= INPUT10 		? '0':'1';
	inputs[10]= INPUT11 	? '0':'1';
	inputs[11]= INPUT12 	? '0':'1';
	INPUT_TRACE("<%.12s>",inputs);	
}



