/**
  \file SysControl.c
														20180105 ԭ����λ�����жϷ�ʽ δ����
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
//��ʼ��������--�ֶ�һ�� PCʹ���´�	ָʾ��
void KeySTART_EXE(void)
{
	if(KeyRESET_ON==0)											//����ڸ�λ�У�START��Ч
	{
		if( INPUT1==0 && KeySTART_ON==0 )					
		{
			delay_ms(20);		
			if( INPUT1==0 )											 //��ʼ����
			{
				 OUTPUT1=1;												 //LED
				 while(INPUT1==0) ; 							 //�ȴ��ɿ�				
				 KeySTART_ON=1;								 //��ʼ����							
				 //RGB_G_ON();											 //RGB
				 KeyRESET_ON=0;								 //�����λ����
				 OUTPUT2=0;				
				 UART_SendStr("START<1>\n>>");
			}
		}
	}
}

//��ͣ��������
u8 	KeyEmergency_EXE(void)
{
	if( INPUT3==1 && KeyEmergency_ON==0 )//���¼�ͣ
	{
		delay_ms(20);
		if( INPUT3==1 )													//ȥ����
		{
			u8 i;
			KeyEmergency_ON=1;											//������־			
			for(i=0;i<N_O_MOTORS;i++)								//���ֹͣ
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
	if(INPUT3==0) 														//�ɿ����� �������
	{
		if(KeyEmergency_ON==1) 
		{		
			if(MotorIsHomed) 	{RGB_B_ON();}		else	{RGB_OFF();}
		}
		KeyEmergency_ON=0;
	} 
	return KeyEmergency_ON;
}

//��λʹ��START,�����ԭ��
void KeyRESET_EXE_ONLINE(void)
{
	if( INPUT2==0 && KeyRESET_ON==0)			//��һ�ΰ�����Ч
	{
		delay_ms(20);
		if( INPUT2==0 )													//ȥ����
		{

			KeySTART_ON=0;										  //�����ʼ����			
			OUTPUT1=0;
			
			RGB_B_ON();
			MotorHoming();												
		}
	}
}
//
void KeyRESET_EXE_OFFLINE(void)
{
	if( INPUT2==0 && KeyRESET_ON==0)			//��һ�ΰ�����Ч
	{
		delay_ms(20);
		if( INPUT2==0 )													//ȥ����
		{
			KeySTART_ON=0;										  //�����ʼ����			
			OUTPUT1=0;
			Cylinder_Reset_check();
			MotorHoming();												
		}
	}	
}
//
void KeyRESET_EXE_IAC_LCR(void)
{
	if( INPUT2==0 && KeyRESET_ON==0)			//��һ�ΰ�����Ч
	{
		delay_ms(20);
		if( INPUT2==0 )													//ȥ����
		{
			KeySTART_ON=0;										  //�����ʼ����			
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

//���û�ԭ���ٶ�ģʽ�µ�����ٶȺͼ��ٶ�
	Write429Int(IDX_VMAX|MOTOR0, 2047);								
	SetAMax(0, 1000);
	Write429Int(IDX_VMAX|MOTOR1, 2047);
	SetAMax(1, 1000);
	Write429Int(IDX_VMAX|MOTOR2, 2047);
	SetAMax(2, 1000);

	UART_Printf("motor homing...\n");
	RGB_B_ON ();		
	
	SensorCheck[0]=0;	SensorCheck[1]=0;	SensorCheck[2]=0;
	RotateLeft(AXIS_Y,SpeedForHoming[AXIS_Y]);		//��ʱ����ת		L		
	MotorIsHoming[AXIS_Y]=1;
}

static void SetUpAfterHomed(u8 axis) 
{
	//֮ǰ�Ѿ������ٶ�ģʽ
	Write429Zero(IDX_VTARGET|(axis<<5));
	delay_ms(100);
	Write429Int(IDX_XACTUAL|(axis<<5), 0);
	Write429Int(IDX_XTARGET|(axis<<5), 0);
}
void MotorHoming_Button(void)
{
	if(KeyRESET_ON==1)													//��ԭ��ʹ��
	{
		u8 SwitchStatus;
		static u8 DoOnce=0;
		static U32_T SensorDelay;
		if(DoOnce==0) {SensorDelay=GetSysTimer(); DoOnce=1;}
		if((MotorIsHoming[AXIS_Y]==1)&&(INPUT6==1))	//Y�ᵽ��ԭ��λ����δ��λ��
		{
			if((Read429Short(IDX_VACTUAL|(1<<5)))>0)	  //Y���ԭ�㷽����ȷ			
			{
				SetUpAfterHomed(AXIS_Y);						  		//ԭ���������
				UART_Printf("Axis Y Homed<OK>\n>>");				//��ӡ��ʾ
				RGB_B_ON ();	
				MotorIsHoming[AXIS_Y]=0;									//Y�������ԭ��
				MotorIsHoming[AXIS_Z]=1;									//Z�Ὺʼ��ԭ��
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
		SwitchStatus=Read429SingleByte(IDX_REF_SWITCHES, 3); 							 //��ȡ��λ״̬	
		if(((SwitchStatus& (0x02<<AXIS_X*2)) ? 1:0)	&& (MotorIsHoming[AXIS_X]))//REFL1
		{
			//Ӳ����λ�Զ�ֹͣ
			RotateRight(AXIS_X,SpeedForHoming[AXIS_X]);		//˳ʱ�� ��ת R
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
		if(MotorIsHoming[AXIS_X]==0 && MotorIsHoming[AXIS_Y]==0 && MotorIsHoming[AXIS_Z]==0) //�������ԭ�����
		{
			KeyRESET_ON=0;	OUTPUT2=0;
			UART_Printf("All Axis Homed<OK>\n>>");
			KeySTART_ON=0;	OUTPUT1=0;
			MotorIsHomed=1;
			RGB_G_ON();
			//���ø�λ���Ĭ�������ٶ�����ٶ�	
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
	if(USART_RX_STA&0x8000)									//���յ�\r\n						
	{
		if(( KeyEmergency_ON ) && (ProID== BUTTON_ONLINE ) )	UART_SendStr("Emergency is pressed\n");	
		else
		{
			len=USART_RX_STA&0x3fff;							//�õ��˴ν��յ������ݳ���
			USART_RX_BUF[len]='\0';								//��ĩβ���������. 
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
			/* #1 �ȸ������������˶�Ѱ�Ҹ���λ�� */
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
			/* #2 �ȴ���λ�������������˶� */
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
			/* #3 �ȴ���λ��������������˶�Ѱ�Ҹ���λ��  */
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
			/* #4 �ȴ���λ����������λΪԭ��  */
			case 4:
							if((Read429SingleByte(IDX_REF_SWITCHES, 3) & (0x02<<AXIS_Y*2)) ? 1:0 )
							{
								SetUpAfterHomed(AXIS_Y);						  														//ԭ���������
								UART_Printf("Axis Y Homed<OK>\n");																//��ӡ��ʾ	
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
	if((MotorIsHoming[AXIS_Y]==1)&&(INPUT6==1))	//Y�ᵽ��ԭ��λ����δ��λ��
	{
		if((Read429Short(IDX_VACTUAL|(1<<5)))>0)	  //Y���ԭ�㷽����ȷ			
		{
			SetUpAfterHomed(AXIS_Y);						  		//ԭ���������
			UART_Printf("Axis Y Homed<OK>\n>>");				//��ӡ��ʾ
				
			MotorIsHoming[AXIS_Y]=0;									//Y�������ԭ��
			MotorIsHoming[AXIS_Z]=1;									//Z�Ὺʼ��ԭ��
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
	EXTI_ClearITPendingBit(EXTI_Line1);  //���LINE4�ϵ��жϱ�־λ  
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

