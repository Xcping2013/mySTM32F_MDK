/**	
  \file 	  main.c
  \author 	Xcping2016	2016-5-10			2018-1-5
  \version 	1.00
  \brief   	Main file
*/
#include <stdlib.h>		//abs strtol
#include "main.h"			//mcu periphery bsp
#include "Globals.h"	//global variable ;fun 
#include "bits.h"

#include "SysTick.h"	
#include "UART.h"
#include "Timer.h"
#include "IO.h"

#include "SPI.h"
#include "TMC429.h"
#include "TMC262.h"

#include "Eeprom.h"
#include "Commands.h"
#include "SysControl.h"

#include "button.h"
#include "lidopen.h"
#include "JABIL_LCR.h"
//NO74HC04
//SYSCLK/1 =72MHZ
UCHAR ProID=LIDOPEN;
UCHAR Board_debugFlag=0;

int main(void)
{ 
  u32 	baud;
  U32_T BlinkDelay;
//basic initial
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  EnableInterrupts();
  InitSysTick();
//board initial
  InitPinAsGPIO();
  AT24CXX_Init();	

  TIM3_PWM_Init(5,0);	 								//PWM=72/6=12M for clock output   TMC429时钟源
  TIM_SetCompare3(TIM3,2);						//占空比
	//set baud 
  if(DefaultSettingsToEEPROM())	baud=AT24CXX_ReadLenByte(ADDR_BT,4);else baud=115200;
  InitUART(baud);
	//get project id and set param for it
	switch(AT24CXX_ReadOneByte(ADDR_PROJECT_ID))
	{
		case BUTTON_ONLINE:	ProID=BUTTON_ONLINE,	RampInit_Button();	UART_SendStr(" Fixture Type=BUTTON ONLINE\n>>");	//EXTIX_Init();
				break ;
		case BUTTON_OFFLINE:ProID=BUTTON_OFFLINE, RampInit_Button();	UART_SendStr(" Fixture Type=BUTTON OFFLINE\n>>"); //EXTIX_Init();
				break ;
		case LIDOPEN:				ProID=LIDOPEN,				RampInit_Lidopen();	UART_SendStr(" Fixture Type=LIDOPEN\n>>");	
				break ;
		case IAC_LCR:				ProID=IAC_LCR,														UART_SendStr(" Fixture Type=IAC LCR\n>>");
				break ;
		case JABIL_LCR:			ProID=JABIL_LCR,		SW_init_JABIL_LCR();	UART_SendStr(" Fixture Type=JABIL LCR\n>>");	RGB_B_ON();	
				break ;
//		default :						AT24CXX_WriteOneByte(ADDR_PROJECT_ID, LIDOPEN);
//												ProID=LIDOPEN,				RampInit_Lidopen();	UART_SendStr(" Fixture Type=LIDOPEN\n>>");	
//				break;	
		default :						AT24CXX_WriteOneByte(ADDR_PROJECT_ID, BUTTON_OFFLINE);			
												ProID=BUTTON_OFFLINE, RampInit_Button();	UART_SendStr(" Fixture Type=BUTTON OFFLINE\n>>");
				break ;
	}
	//initial motor controller
	InitSPI();
  Init429();		
	//原点输入改为外部中断
	
	delay_ms(300);	//等待其他外围设备上电初始化OK
	UART_SendStr("Fixture Init Ok\n>>");
  BlinkDelay=GetSysTimer();
  
  for(;;)
  {		
		//board auto operate from hardware or respond cmd according to project
		switch(ProID)
		{
			case BUTTON_ONLINE:										//线上完整DUT 带通讯 无气缸 IO控制急停
					 //if(KeyEmergency_EXE()==0)	
					 { 
					   KeySTART_EXE();								//双启板子带锁存功能		
				  	 KeyRESET_EXE_ONLINE();
					   MotorHoming_Button();						//电机回位
					 }
					 break ;		
			case BUTTON_OFFLINE:							  	//线下外壳DUT 无通讯 有气缸
					 KeySTART_EXE();									//双启板子带锁存功能
					 KeyRESET_EXE_OFFLINE();						//气缸回位
					 MotorHoming_Button();							//电机回位
					 break ;
			case LIDOPEN: 												
					 if(KeyS_actEN_LidOpen)						//双启控制气缸进出和门开关
					 {
						 KeyS_act_LIDOPEN();
					 }
					 MotorHoming_Lidopen();							//电机回位		
					 break;
			case IAC_LCR:													
					 KeySTART_EXE();									//双启板子带锁存功能
					 KeyRESET_EXE_IAC_LCR();							//气缸回位
					 break;	
			case JABIL_LCR:
					if(KeyS_actEN_LCR)								//双启控制气缸进出与下压
					 {
						 KeyS_act_JABIL_LCR();
					 }
					 break;
			
			default:
					 break ;		
		}
		//PC send cmd to controller 
		CommandCheckAndExe();
		//running indicate
  	if(abs((int)(GetSysTimer()-BlinkDelay))>350)
  	{
  		LED1_TOGGLE();
  		BlinkDelay=GetSysTimer();
  	}
		//board self-test
		if(Board_debugFlag==1)
		{
			static U32_T BlinkDelay_IO;
			static	u8 IO_debug=0;
			GPIO_InitTypeDef  GPIO_InitStructure;
			if(IO_debug==0)
			{
				IO_debug=1;
				
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
				//IN1~5																											    
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				//IN6~12																										    
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_Init(GPIOE, &GPIO_InitStructure);
				
			}
			if(abs((int)(GetSysTimer()-BlinkDelay_IO))>1000)
			{
				PBout(5)=1;PBout(7)=1;PBout(9)=1;PEout(1)=1;PEout(3)=1;PEout(5)=1;
				PBout(6)=0;PBout(8)=0;PEout(0)=0;PEout(2)=0;PEout(4)=0;PEout(6)=0;
				
				OUTPUT1=1;OUTPUT3=1;OUTPUT5=1;OUTPUT7=1;
				OUTPUT2=0;OUTPUT4=0;OUTPUT6=0;OUTPUT8=0;
				
				BlinkDelay_IO=GetSysTimer();
			}
			else if(abs((int)(GetSysTimer()-BlinkDelay_IO))>500) 
			{
				PBout(5)=0;PBout(7)=0;PBout(9)=0;PEout(1)=0;PEout(3)=0;PEout(5)=0;
				PBout(6)=1;PBout(8)=1;PEout(0)=1;PEout(2)=1;PEout(4)=1;PEout(6)=1;
				
				OUTPUT1=0;OUTPUT3=0;OUTPUT5=0;OUTPUT7=0;
				OUTPUT2=1;OUTPUT4=1;OUTPUT6=1;OUTPUT8=1;
			}
			
			//Debug_In_Out();			
		}
  }
}
