#include "exti.h"
#include "delay.h"
#include "usart.h"
#include "motor_motion.h"

//#define INPUT10	PEin(4)	ÏÂ½µÑØ
//#define INPUT11	PEin(5)
//#define INPUT12	PEin(6)
void EXTI_MotorORG1_DeInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;								
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;							
	NVIC_Init(&NVIC_InitStructure); 
}
void EXTI_MotorORG23_DeInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;								
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;							
	NVIC_Init(&NVIC_InitStructure); 
}
void EXTI_MotorORG1_Init(void)
{
 	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);

	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	if(act_ORG==LOW)  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	else 								EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;					
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;			
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
	NVIC_Init(&NVIC_InitStructure); 
}

void EXTI_MotorORG2_Init(void)
{
 	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource5);

	EXTI_InitStructure.EXTI_Line=EXTI_Line5;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	if(act_ORG==LOW)  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	else 								EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;					
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;			
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
	NVIC_Init(&NVIC_InitStructure); 
}

void EXTI_MotorORG3_Init(void)
{
 	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource6);

	EXTI_InitStructure.EXTI_Line=EXTI_Line6;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	if(act_ORG==LOW)  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	else 								EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;					
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;			
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
	NVIC_Init(&NVIC_InitStructure); 
}

void MotorORG_TI_Enable(u8 Motor)
{
	if(Motor==1)	EXTI_MotorORG1_Init();
	if(Motor==2)	EXTI_MotorORG2_Init();
	if(Motor==3)	EXTI_MotorORG3_Init();
}
void MotorORG_TI_Disable(u8 Motor)
{
	if(Motor==1)	EXTI_MotorORG1_DeInit();
	if(Motor==2)	EXTI_MotorORG23_DeInit();
	if(Motor==3)	EXTI_MotorORG23_DeInit();
}

void EXTI4_IRQHandler(void)
{
		exit4_handler();
		EXTI_ClearITPendingBit(EXTI_Line4); 
}
 
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
			exit5_handler();
			EXTI_ClearITPendingBit(EXTI_Line5);
	}
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
			exit6_handler();
			EXTI_ClearITPendingBit(EXTI_Line6);
	}
}

 
