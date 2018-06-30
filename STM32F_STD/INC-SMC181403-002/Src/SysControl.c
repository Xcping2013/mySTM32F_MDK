/**
  \file SysControl.c
														20180105 原点限位采用中断方式 未处理
  \version 1.00

  \brief Motor monitoring

  This file contains the SystemControl function which does all necessary motor
  monitoring tasks.
*/

#include <limits.h>
#include <stdlib.h>
#include "bits.h"
#include "main.h"
#include "Globals.h"
#include "IO.h"
#include "SysTick.h"
#include "TMC429.h"
#include "Commands.h"
#include "SysControl.h"
#include "UART.h"
#include "TMC262.h"
#include "EEPROM.h"

u8 	KeySTART_ON=0;
u8 	KeyEmergency_ON=0;
u8 	KeyRESET_ON=0;

static u16  SpeedForHoming[3]={839,839,839};
static u8   MotorIsHoming[3]={0};	
u8 	MotorIsHomed=0;

static u8 SensorCheck[3]={0,0,0};
static void RotateRight(u8 axis, int velocity);
static void RotateLeft(u8 axis, int velocity);
//-----------------------------------------------------------Lidopen
u8 	HomeStep=0;
static void SetUpAfterHomed(u8 axis);
static void SetAmaxAuto(u8 ActualMotor,int speed);
//-----------------------------------------------------------Lidopen
//开始按键动作--手动一次 PC使能下次	指示灯
void KeySTART_EXE(void)
{
	if(KeyRESET_ON==0)											//电机在复位中，START无效
	{
		if( INPUT1==0 && KeySTART_ON==0 )					
		{
			delay_ms(20);		
			if( INPUT1==0 )											 //开始按键
			{
				 OUTPUT1=1;												 //LED
				 while(INPUT1==0) ; 							 //等待松开				
				 KeySTART_ON=1;								 //开始自锁							
				 //RGB_G_ON();											 //RGB
				 KeyRESET_ON=0;								 //解除复位自锁
				 OUTPUT2=0;				
				 UART_SendStr("START<1>\n>>");
			}
		}
	}
}

//急停按键动作
u8 	KeyEmergency_EXE(void)
{
	if( INPUT3==1 && KeyEmergency_ON==0 )//按下急停
	{
		delay_ms(20);
		if( INPUT3==1 )													//去抖动
		{
			u8 i;
			KeyEmergency_ON=1;											//自锁标志			
			for(i=0;i<N_O_MOTORS;i++)								//电机停止
			{
//				Set429RampMode(i, RM_VELOCITY);
//				Write429Zero(IDX_VTARGET|(i<<5));
				HardStop(i);
				SetUpAfterHomed(i);
			}
			RGB_R_ON();															//RGB
			KeySTART_ON=0;	KeyRESET_ON=0;
			OUTPUT1=0;	OUTPUT2=0;									//LED			
		}
	}
	if(INPUT3==0) 														//松开按键 解除自锁
	{
		if(KeyEmergency_ON==1) 
		{		
			if(MotorIsHomed) 	{RGB_B_ON();}		else	{RGB_OFF();}
		}
		KeyEmergency_ON=0;
	} 
	return KeyEmergency_ON;
}

//复位使能START,电机回原点
void KeyRESET_EXE_ONLINE(void)
{
	if( INPUT2==0 && KeyRESET_ON==0)			//第一次按键有效
	{
		delay_ms(20);
		if( INPUT2==0 )													//去抖动
		{

			KeySTART_ON=0;										  //解除开始自锁			
			OUTPUT1=0;
			
			RGB_B_ON();
			MotorHoming();												
		}
	}
}
//
void KeyRESET_EXE_OFFLINE(void)
{
	if( INPUT2==0 && KeyRESET_ON==0)			//第一次按键有效
	{
		delay_ms(20);
		if( INPUT2==0 )													//去抖动
		{
			KeySTART_ON=0;										  //解除开始自锁			
			OUTPUT1=0;
			Cylinder_Reset_check();
			MotorHoming();												
		}
	}	
}
//
void KeyRESET_EXE_IAC_LCR(void)
{
	if( INPUT2==0 && KeyRESET_ON==0)			//第一次按键有效
	{
		delay_ms(20);
		if( INPUT2==0 )													//去抖动
		{
			KeySTART_ON=0;										  //解除开始自锁			
			OUTPUT1=0;
			Cylinder_Reset_check();										
		}
	}	
}
//
static void RotateRight(u8 axis, int velocity)
{
	Set429RampMode(axis, RM_VELOCITY);
	Write429Short(IDX_VTARGET|(axis<<5), velocity);
}
//
static void RotateLeft(u8 axis, int velocity)
{
	Set429RampMode(axis, RM_VELOCITY);
	Write429Short(IDX_VTARGET|(axis<<5), -velocity);
}
//
void MotorHoming(void)
{
	KeyRESET_ON=1;	MotorIsHomed=0;															
	OUTPUT2=1;

//设置回原点速度模式下的最大速度和加速度
	Write429Int(IDX_VMAX|MOTOR0, 2047);								
	SetAMax(0, 1000);
	Write429Int(IDX_VMAX|MOTOR1, 2047);
	SetAMax(1, 1000);
	Write429Int(IDX_VMAX|MOTOR2, 2047);
	SetAMax(2, 1000);

	UART_Printf("motor homing...\n");
	RGB_B_ON ();		
	
	SensorCheck[0]=0;	SensorCheck[1]=0;	SensorCheck[2]=0;
	RotateLeft(AXIS_Y,SpeedForHoming[AXIS_Y]);		//逆时针旋转		L		
	MotorIsHoming[AXIS_Y]=1;
}

static void SetUpAfterHomed(u8 axis) 
{
	//之前已经处于速度模式
	Write429Zero(IDX_VTARGET|(axis<<5));
	delay_ms(100);
	Write429Int(IDX_XACTUAL|(axis<<5), 0);
	Write429Int(IDX_XTARGET|(axis<<5), 0);
}
void MotorHoming_Button(void)
{
	if(KeyRESET_ON==1)													//回原点使能
	{
		u8 SwitchStatus;
		static u8 DoOnce=0;
		static U32_T SensorDelay;
		if(DoOnce==0) {SensorDelay=GetSysTimer(); DoOnce=1;}
		if((MotorIsHoming[AXIS_Y]==1)&&(INPUT6==1))	//Y轴到达原点位置且未复位过
		{
			if((Read429Short(IDX_VACTUAL|(1<<5)))>0)	  //Y轴回原点方向正确			
			{
				SetUpAfterHomed(AXIS_Y);						  		//原点参数设置
				UART_Printf("Axis Y Homed<OK>\n>>");				//打印提示
				RGB_B_ON ();	
				MotorIsHoming[AXIS_Y]=0;									//Y轴结束回原点
				MotorIsHoming[AXIS_Z]=1;									//Z轴开始回原点
				RotateLeft(AXIS_Z,SpeedForHoming[AXIS_Z]);
			}
		}
	
		if((MotorIsHoming[AXIS_Z]==1)&&(INPUT7==1))
		{
			if((Read429Short(IDX_VACTUAL|(2<<5)))>0)
			{
				SetUpAfterHomed(AXIS_Z);	
				UART_Printf("Axis Z Homed<OK>\n>>");
				RGB_B_ON ();
				MotorIsHoming[AXIS_Z]=0;
				MotorIsHoming[AXIS_X]=1;
				RotateRight(AXIS_X,SpeedForHoming[AXIS_X]);			
			}
		}
		
		if((MotorIsHoming[AXIS_X]==1)&&(INPUT5==1))
		{
			if((Read429Short(IDX_VACTUAL|(0<<5)))<0)
			{
				SetUpAfterHomed(AXIS_X);
				UART_Printf("Axis X Homed<OK>\n>>");				
				MotorIsHoming[AXIS_X]=0;
			}
		}
		SwitchStatus=Read429SingleByte(IDX_REF_SWITCHES, 3); 							 //读取限位状态	
		if(((SwitchStatus& (0x02<<AXIS_X*2)) ? 1:0)	&& (MotorIsHoming[AXIS_X]))//REFL1
		{
			//硬件限位自动停止
			RotateRight(AXIS_X,SpeedForHoming[AXIS_X]);		//顺时针 正转 R
			if(SensorCheck[0]==1) SensorCheck[0]=2;  
		}
		if(((SwitchStatus& (0x01<<AXIS_X*2)) ? 1:0)	&& (MotorIsHoming[AXIS_X]))//REFR1
		{
			RotateLeft(AXIS_X,SpeedForHoming[AXIS_X]);		
			if(SensorCheck[0]==0) SensorCheck[0]=1; 
			else if(SensorCheck[0]==2) SensorCheck[0]=3; 
		}
	
		if(((SwitchStatus& (0x02<<AXIS_Y*2)) ? 1:0)	&& (MotorIsHoming[AXIS_Y]))//L2
		{
			RotateRight(AXIS_Y,SpeedForHoming[AXIS_Y]);		
			if(SensorCheck[1]==0) SensorCheck[1]=1; 
			else if(SensorCheck[1]==2) SensorCheck[1]=3; 
		}
		if(((SwitchStatus& (0x01<<AXIS_Y*2)) ? 1:0)	&& (MotorIsHoming[AXIS_Y]))//R2
		{
			RotateLeft(AXIS_Y,SpeedForHoming[AXIS_Y]);		
			if(SensorCheck[1]==1) SensorCheck[1]=2;  
		}

		if(((SwitchStatus& (0x02<<AXIS_Z*2)) ? 1:0)	&& (MotorIsHoming[AXIS_Z]))//L3
		{		
			RotateRight(AXIS_Z,SpeedForHoming[AXIS_Z]);		
			if(SensorCheck[2]==0) SensorCheck[2]=1; 
			else if(SensorCheck[2]==2) SensorCheck[2]=3; 
		}
		if(((SwitchStatus& (0x01<<AXIS_Z*2)) ? 1:0	) && (MotorIsHoming[AXIS_Z]))//R3
		{
			RotateLeft(AXIS_Z,SpeedForHoming[AXIS_Z]);		
			if(SensorCheck[2]==1) SensorCheck[2]=2; 
		}
		if(MotorIsHoming[AXIS_X]==0 && MotorIsHoming[AXIS_Y]==0 && MotorIsHoming[AXIS_Z]==0) //所有轴回原点完成
		{
			KeyRESET_ON=0;	OUTPUT2=0;
			UART_Printf("All Axis Homed<OK>\n>>");
			KeySTART_ON=0;	OUTPUT1=0;
			MotorIsHomed=1;
			RGB_G_ON();
			//设置复位后的默认运行速度与加速度	
			Write429Int(IDX_VMAX|MOTOR0, 280);MotorConfig[0].VMax=280;
			SetAMax(0, 120);MotorConfig[0].AMax=120;
			Write429Int(IDX_VMAX|MOTOR1, 140);MotorConfig[1].VMax=140;
			SetAMax(1, 30);MotorConfig[1].AMax=30;
			Write429Int(IDX_VMAX|MOTOR2, 280);MotorConfig[2].VMax=280;
			SetAMax(2, 120);MotorConfig[2].AMax=120;
		}
		if(abs((int)(GetSysTimer()-SensorDelay))>1000 )
		{
			if(SensorCheck[0]==3)	{ SensorCheck[0]=0; UART_SendStr("Please check home sensor >>motor X<<\n");	RGB_R_ON();}
			if(SensorCheck[1]==3)	{	SensorCheck[1]=0; UART_SendStr("Please check home sensor >>motor Y<<\n");	RGB_R_ON();}
			if(SensorCheck[2]==3)	{ SensorCheck[2]=0; UART_SendStr("Please check home sensor >>motor Z<<\n");	RGB_R_ON();}
			SensorDelay=GetSysTimer();
		}
	}
}
//
void CommandCheckAndExe(void)
{
	u8 len;
	if(USART_RX_STA&0x8000)									//接收到\r\n						
	{
		if(( KeyEmergency_ON ) && (ProID== BUTTON_ONLINE ) )	UART_SendStr("Emergency is pressed\n");	
		else
		{
			len=USART_RX_STA&0x3fff;							//得到此次接收到的数据长度
			USART_RX_BUF[len]='\0';								//在末尾加入结束符. 
			UART_SendStr((char*)USART_RX_BUF);
			UART_SendChar('\n');
			if(USART_RX_BUF[0]=='\0')	
				UART_SendStr("\n");
			else if(ProcessCommand((char*)USART_RX_BUF))	;
			else 
				UART_SendStr("<NA>\n");								
		}
		UART_SendStr(">>");	
		USART_RX_STA=0;
	}	
}
//-----------------------------------------------------------Lidopen
static void SetAmaxAuto(u8 ActualMotor,int speed)
{
	if (speed<25)
	{		SetAMax(ActualMotor, speed+5);	MotorConfig[ActualMotor].AMax=speed+5;}			
	else if (speed<100)
	{		SetAMax(ActualMotor, speed+10);	MotorConfig[ActualMotor].AMax=speed+10;}
	else if (speed<150)
	{		SetAMax(ActualMotor, speed+30);MotorConfig[ActualMotor].AMax=speed+30;}
	else if (speed<280)
	{		SetAMax(ActualMotor, speed+60);MotorConfig[ActualMotor].AMax=speed+60;}
	else if (speed<420)
	{		SetAMax(ActualMotor, speed+100);MotorConfig[ActualMotor].AMax=speed+100;}
	else if (speed<560)
	{		SetAMax(ActualMotor, speed+150);MotorConfig[ActualMotor].AMax=speed+150;}
	else if (speed<700)
	{		SetAMax(ActualMotor, speed+200);MotorConfig[ActualMotor].AMax=speed+200;}
	else if (speed<840)
	{		SetAMax(ActualMotor, speed+250);MotorConfig[ActualMotor].AMax=speed+250;}
	else if (speed<980)
	{		SetAMax(ActualMotor, speed+300);MotorConfig[ActualMotor].AMax=speed+300;}
	else if (speed<=1180)
	{		SetAMax(ActualMotor, speed+350);MotorConfig[ActualMotor].AMax=speed+350;}
	else if (speed<2047)
	{		SetAMax(ActualMotor, speed+600);MotorConfig[ActualMotor].AMax=speed+600;}	
}
void MotorHoming_Lidopen(void)
{
	if(	HomeStep )
	{
		u8 i;
		static Data_4Bytes Speed1;
		static Data_4Bytes Speed2;
		static Data_4Bytes Speed3;
		switch(HomeStep)
		{
			/* #1 先高速匀速向左运动寻找负限位点 */
			case 1:	
							PAout(2)=1;
							Write429Int(IDX_VMAX|MOTOR1, 300);								
							SetAMax(AXIS_Y, 300);
							UART_Printf("motor homing...\n");		
							for(i=0;i<4;i++)
							{
								Speed1.Byte[i]=AT24CXX_ReadOneByte(1*4+100+i);
							}
							SetAmaxAuto(AXIS_Y, (int)Speed1.f32);
							RotateLeft(AXIS_Y,(int)Speed1.f32);
							HomeStep=2;
							
				break;
			/* #2 等待到位后低速向右相对运动 */
			case 2:
							if((Read429SingleByte(IDX_REF_SWITCHES, 3) & (0x02<<AXIS_Y*2)) ? 1:0 )
							{
								delay_ms(100);
								for(i=0;i<4;i++)
								{
									Speed2.Byte[i]=AT24CXX_ReadOneByte(2*4+100+i);
								}
								Write429Short(IDX_VMAX|AXIS_Y<<5, (int)Speed2.f32);
								SetAmaxAuto(AXIS_Y, (int)Speed2.f32);
								Set429RampMode(AXIS_Y, RM_VELOCITY);
								Write429Int(IDX_XTARGET|(AXIS_Y<<5), Read429Int(IDX_XACTUAL|(AXIS_Y<<5))+1000);
								Set429RampMode(AXIS_Y, RM_RAMP);
								HomeStep=3;
							}
				break;
			/* #3 等待到位后低速匀速向左运动寻找负限位点  */
			case 3:
							if(((Read429Status() & (0x01<<AXIS_Y*2)) ? 1:0) )
							{
								delay_ms(100);
								for(i=0;i<4;i++)
								{
									Speed3.Byte[i]=AT24CXX_ReadOneByte(3*4+100+i);
								}
								SetAmaxAuto(AXIS_Y, (int)Speed3.f32);
								RotateLeft(AXIS_Y,(int)Speed3.f32);	
								HomeStep=4;
							}
				break;
			/* #4 等待到位并设置左限位为原点  */
			case 4:
							if((Read429SingleByte(IDX_REF_SWITCHES, 3) & (0x02<<AXIS_Y*2)) ? 1:0 )
							{
								SetUpAfterHomed(AXIS_Y);						  														//原点参数设置
								UART_Printf("Axis Y Homed<OK>\n");																//打印提示	
								MotorIsHomed=1;
								HomeStep=0;
							}
				break;
			default :
				break;
		}
	}
}
//-----------------------------------------------------------add home exit 
void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9);

  EXTI_InitStructure.EXTI_Line=EXTI_Line9;	
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	 	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource0);
  EXTI_InitStructure.EXTI_Line=EXTI_Line0;	
	EXTI_Init(&EXTI_InitStructure);	 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource1);
  EXTI_InitStructure.EXTI_Line=EXTI_Line1;	
	EXTI_Init(&EXTI_InitStructure);	 	

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x07;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;				
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
	NVIC_Init(&NVIC_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x06;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;				
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
	NVIC_Init(&NVIC_InitStructure); 

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
	NVIC_Init(&NVIC_InitStructure);
}
//
void EXTI0_IRQHandler(void)
{
	//OUTPUT6=~OUTPUT6;
	if((MotorIsHoming[AXIS_Y]==1)&&(INPUT6==1))	//Y轴到达原点位置且未复位过
	{
		if((Read429Short(IDX_VACTUAL|(1<<5)))>0)	  //Y轴回原点方向正确			
		{
			SetUpAfterHomed(AXIS_Y);						  		//原点参数设置
			UART_Printf("Axis Y Homed<OK>\n>>");				//打印提示
				
			MotorIsHoming[AXIS_Y]=0;									//Y轴结束回原点
			MotorIsHoming[AXIS_Z]=1;									//Z轴开始回原点
			RotateLeft(AXIS_Z,SpeedForHoming[AXIS_Z]);
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line0); 
}
void EXTI1_IRQHandler(void)
{
	//OUTPUT7=~OUTPUT7;
	if((MotorIsHoming[AXIS_Z]==1)&&INPUT7==1)
	{
		if((Read429Short(IDX_VACTUAL|(2<<5)))>0)
		{
			SetUpAfterHomed(AXIS_Z);	
			UART_Printf("Axis Z Homed<OK>\n>>");
			
			MotorIsHoming[AXIS_Z]=0;
			MotorIsHoming[AXIS_X]=1;
			RotateRight(AXIS_X,SpeedForHoming[AXIS_X]);			
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line1);  //清除LINE4上的中断标志位  
}
void EXTI9_5_IRQHandler(void)
{
	//OUTPUT5=~OUTPUT5;
		if((MotorIsHoming[AXIS_X]==1)&&(INPUT5==1))
	{
		if((Read429Short(IDX_VACTUAL|(0<<5)))<0)
		{
			SetUpAfterHomed(AXIS_X);
			UART_Printf("Axis X Homed<OK>\n>>");				
			MotorIsHoming[AXIS_X]=0;
		}
	}	
	EXTI_ClearITPendingBit(EXTI_Line9);  
}
//
//20180118
void Cylinder_Reset_check(void)
{
	U32_T DebugDelay;
	DebugDelay=GetSysTimer();		
	UART_SendStr("\n>>Cylinder resetting...\n");
	OUTPUT5=0;OUTPUT6=1;delay_ms(100);
	while(	(INPUT8==1 && INPUT9==0) ? 0:1 )	
	{
		if(abs((int)(GetSysTimer()-DebugDelay))>1000) 
		{
			UART_SendStr("Please check pogo pin cylinder is >>BACK<< :  OUT6=1,OUT5=0;    IN8=0,IN9=1\n");
			LED1_TOGGLE();RGB_R_ON();
			DebugDelay=GetSysTimer();
		}
	}
	OUTPUT8=0;OUTPUT7=1;delay_ms(100);
	while((INPUT11==1 && INPUT10==0) 	? 0:1 )	
	{
		if(abs((int)(GetSysTimer()-DebugDelay))>1000) 
		{
			UART_SendStr("Please check pogo pin cylinder is >>UPSIDE<< : OUT7=1,OUT8=0;   IN11=0,IN10=1\n");
			LED1_TOGGLE();RGB_R_ON();
			DebugDelay=GetSysTimer();}
	}
	OUTPUT3=0;OUTPUT4=1;delay_ms(100);
	while((INPUT3==1 && INPUT4==0) 		? 0:1 )	
	{
		if(abs((int)(GetSysTimer()-DebugDelay))>1000) 
		{
			UART_SendStr("Please check dut cylinder is >>OUTSIDE<< : OUT4=1,OUT3=0;    IN3=0,IN4=1\n");
			LED1_TOGGLE();RGB_R_ON();
			DebugDelay=GetSysTimer();
		}
	}
	RGB_B_ON();
}
//

