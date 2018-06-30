#include "stdio.h"	
#include "OUTPUT.h"

void output_init(void)
{
  // GPIO init
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	//OUT1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);		
	//OUT2~4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;		       					
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	GPIO_ResetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);	
	//OUT5~8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;		       					
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	GPIO_ResetBits(GPIOD,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);	
	//SYS_LED+RGB
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4;																		
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	GPIO_ResetBits(GPIOD,GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4); 

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

	//POSCMP1 INT1  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;									
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}


//
void output_write(uint8_t channel, uint8_t value)
{
	static uint8_t Out9_14_Inited;
	if(Out9_14_Inited==0&&(channel>8))
	{
		GPIO_InitTypeDef  GPIO_InitStructure;
		Out9_14_Inited=1;
 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		//OUT9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);		
		//OUT10~12
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_13|GPIO_Pin_11;	       					
		GPIO_Init(GPIOD, &GPIO_InitStructure);	
		GPIO_ResetBits(GPIOD,GPIO_Pin_10|GPIO_Pin_13|GPIO_Pin_11);			
		//OUT13~14
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;																		
		GPIO_Init(GPIOE, &GPIO_InitStructure);	
		GPIO_ResetBits(GPIOE,GPIO_Pin_14|GPIO_Pin_15); 	
		//LED
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;																		
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		GPIO_ResetBits(GPIOB,GPIO_Pin_12); 	
	}
	if(0<channel && channel< 15)
	{
		switch(channel)
		{
			case 1: 	OUTPUT1=value; 		break;
			case 2: 	OUTPUT2=value; 		break;		
			case 3: 	OUTPUT3=value; 		break;	
			case 4: 	OUTPUT4=value; 		break;		
			case 5: 	OUTPUT5=value; 		break;
			case 6: 	OUTPUT6=value; 		break;
			case 7: 	OUTPUT7=value; 		break;
			case 8: 	OUTPUT8=value; 		break;		
			case 9: 	OUTPUT9=value; 		break;	
			case 10: 	OUTPUT10=value; 	break;		
			case 11: 	OUTPUT11=value; 	break;
			case 12: 	OUTPUT12=value; 	break;
			case 13: 	OUTPUT13=value; 	break;
			case 14: 	OUTPUT14=value; 	break;	

			default:										break;
		}	
		printf("<ok>");
	}
	else printf("<channel must be 1~14>");
}
//
