/**
  \Project		IN3-SMC-3
	\file 			IO.c
  \author 		Xcping2016
  \version 		1.00

  \brief I/O functions

  This file provides functions for intializing and
  using the I/O ports and some other miscellaneous
  stuff.
*/


#include <stdlib.h>
#include "bits.h"
#include "main.h"
#include "IO.h"
#include "UART.h"
#include "systick.h"
#include "TMC429.h"

/*********************************************//**
  \fn 		InitPinAsGPIO(void)
  \brief 	Initialize I/O

  This function initalizes all I/O port pins of
  the CPU that are not initialized in somewhere
  else in other initialization functions.
*************************************************/
void InitPinAsGPIO(void)
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
void SetOutput(uint8_t ch, uint8_t val)
{
	static uint8_t Out9_14_Inited;
	if(Out9_14_Inited==0&&(ch>8))
	{
		GPIO_InitTypeDef  GPIO_InitStructure;
		Out9_14_Inited=1;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 
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
		
	switch(ch)
	{
		case 1: 	OUTPUT1=val; 		break;
		case 2: 	OUTPUT2=val; 		break;		
		case 3: 	OUTPUT3=val; 		break;	
		case 4: 	OUTPUT4=val; 		break;		
		case 5: 	OUTPUT5=val; 		break;
		case 6: 	OUTPUT6=val; 		break;
		case 7: 	OUTPUT7=val; 		break;
		case 8: 	OUTPUT8=val; 		break;		
		case 9: 	OUTPUT9=val; 		break;	
		case 10: 	OUTPUT10=val; 	break;		
		case 11: 	OUTPUT11=val; 	break;
		case 12: 	OUTPUT12=val; 	break;
		case 13: 	OUTPUT13=val; 	break;
		case 14: 	OUTPUT14=val; 	break;	

		default:									break;
	}	
}
//
uint8_t GetInput(uint8_t ch)
{
	static uint8_t In9_14_Inited;
	if(In9_14_Inited==0&&(ch>8))
	{
		GPIO_InitTypeDef  GPIO_InitStructure;
		In9_14_Inited=1;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		//INT13 IN17	IN18
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_15|GPIO_Pin_13;
		GPIO_Init(GPIOB, &GPIO_InitStructure);                                                 
		//INT14~16
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_12|GPIO_Pin_9;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		
	}
	switch(ch)
	{
		case 1: 	return INPUT1 	? 0:1;
		case 2: 	return INPUT2 	? 0:1;
		case 3: 	return INPUT3 	? 0:1;
		case 4: 	return INPUT4 	? 0:1;
		case 5: 	return INPUT5 	? 0:1;
		case 6: 	return INPUT6 	? 0:1;
		case 7: 	return INPUT7 	? 0:1;
		case 8: 	return INPUT8 	? 0:1;
		case 9: 	return INPUT9 	? 0:1;
		case 10: 	return INPUT10	? 0:1;
		case 11: 	return INPUT11 	? 0:1;
		case 12: 	return INPUT12 	? 0:1;
		case 13: 	return INPUT13 	? 0:1;
		case 14: 	return INPUT14 	? 0:1;
		case 15: 	return INPUT15 	? 0:1;
		case 16: 	return INPUT16 	? 0:1;
		case 17: 	return INPUT17 	? 0:1;
		case 18: 	return INPUT18 	? 0:1;
		default:  return 255;
	}	
}


/***************************************************************//**
   \fn EnableInterrupts(void)
   \brief Enable all interrupts

  This function globally enables all interrupts.
********************************************************************/
void EnableInterrupts(void)
{
  __ASM volatile("cpsie i");		 
}


/***************************************************************//**
   \fn DisableInterrupts(void)
   \brief Disable all interrupts

  This function globally disables all interrupts.
********************************************************************/
void DisableInterrupts(void)
{
  __ASM volatile("cpsid i");
}


/***************************************************************//**
   \fn SetMotorCurrent(UCHAR Motor, UCHAR Current)
   \brief Set motor current
   \param Motor    Motor number (with stepRocker always 0)
   \param Current  Motor current scaled to 0..255

   This function sets the motor current, scaled to 0..255 as
   usual in TMCL.
********************************************************************/
void SetMotorCurrent(UCHAR Motor, UCHAR Current)
{
  //Set262StallGuardCurrentScale(WHICH_262(Motor), Current>>3);
}


/***************************************************************//**
   \fn GetStallState(UCHAR Motor)
   \brief Get stall flag state of motor driver
   \param Motor  Motor number (with stepRocker always 0)
   \return  TRUE stall\n
            FALSE no stall

   This function reads the state of the SG_TEST pin of the TMC262.
********************************************************************/
UCHAR GetStallState(UCHAR Motor)
{
  if(Motor==0)
  {
//    if(GPIO1->PDSR & BIT20)
//      return TRUE;
//    else
      return FALSE;
  }
  else return FALSE;
}
//
void ResetCPU(UCHAR ResetPeripherals)
{
  if(ResetPeripherals)
    //NVIC_GenerateSystemReset();
	  SCB->AIRCR = SCB_AIRCR_VECTKEY_Msk | (u32)0x04;

  else
    //NVIC_GenerateCoreReset();
	  SCB->AIRCR = SCB_AIRCR_VECTKEY_Msk | (u32)0x01;
}
void Debug_In_Out(void)
{
	u8 i;
	for(i=1;i<9;i++)
	{
		if(GetInput(i)==0) SetOutput(i,0); else SetOutput(i,1);
	}
	for(i=9;i<19;i++)
	{
		if(GetInput(i)==0) SetOutput(i-8,0); else SetOutput(i-8,1);
	}
	
}
