#include <stdlib.h>
#include "bits.h"
#include "main.h"
#include "Globals.h"
#include "IO.h"
#include "UART.h"
#include "systick.h"

//IN1-START
//IN2-DUT_1			
//IN3-DUT_2     in 1
//IN4-ALARM_IN
//IN5-急停
//IN9-DOOR_1		close 0
//IN10-DOOR_2

//OUT1-DOOR_1    close 1 1
//OUT2-DOOR_2						2 0
//OUT3-DUT_1		 in    3 0
//OUT4-DUT_2
//OUT6-DRIVE_EN
//OUT5-DRIVE_CLEAN
//OUT7-LED
#define DutGoInside()		OUTPUT3=0;OUTPUT4=1//GPIOC->BRR=BIT8;	GPIOC->BSRR=BIT9			//out3=0;out4=1
#define DutGoOutside()	OUTPUT3=1;OUTPUT4=0//GPIOC->BSRR=BIT8;	GPIOC->BRR=BIT9				//out3=1;out4=0
#define CloseTheDoor()	OUTPUT1=1;OUTPUT2=0//GPIOC->BSRR=BIT6;	GPIOC->BRR=BIT7				//out1=1;out2=0
#define OpenTheDoor()		OUTPUT1=0;OUTPUT2=1//GPIOC->BRR=BIT6;	GPIOC->BSRR=BIT7			//out1=0;out2=1

#define PB_START				INPUT1
#define PRESS						0

#define STANDBY 				0
#define DUT_GO_INSIDE 	1
#define CLOSE_DOOR 			2
#define OPEN_DOOR 			3
#define DUT_GO_OUTSIDE 	4

#define	INSIDE					0
#define	OUTSIDE					1
#define	UNDEFINE				3
#define	UPSIDE					0
#define DOWNSIDE				1

uint8_t	OpStep=STANDBY;
uint8_t	KeyS_actOK_LidOpen=0;
uint8_t KeyS_actEN_LidOpen=0;

static UCHAR DutPosition(void);
static UCHAR DoorPosition(void);

void RampInit_Lidopen(void)
{
  uint8_t i;
	for(i=0; i<N_O_MOTORS; i++) 	//默认电机主要参数配置，主要为速度和加速度
	{																  
		MotorConfig[i].VMax=300;    
		MotorConfig[i].AMax=100;
		//12MHZ|10000steps	
		//2047-->17.57/M;					0.29R/S			100--0.01R/S
		MotorConfig[i].PulseDiv=7;	
		MotorConfig[i].RampDiv=10;	
		SpeedChangedFlag[i]=TRUE;
	}
}

void KeyS_act_LIDOPEN(void)												
{
	switch(OpStep)						
	{	
		case STANDBY:
			if(DutPosition()==OUTSIDE && DoorPosition()==UPSIDE )					
			{
				if(PB_START==PRESS) 																								
				{	
					delay_ms(20);
					if(PB_START==PRESS)	OpStep=DUT_GO_INSIDE;					
				}			
			}	
			break;
			
		case DUT_GO_INSIDE:	
			if(PB_START==PRESS)	
			{	
				 OUTPUT7=1;DutGoInside();delay_ms(100);
				 if(DutPosition()==INSIDE )	OpStep=CLOSE_DOOR;	
			}
			else	OpStep=DUT_GO_OUTSIDE;
			break;	
			
		case CLOSE_DOOR:
			if(PB_START==PRESS)	
			{	
				 OUTPUT7=1;CloseTheDoor();delay_ms(100);
				 if(DoorPosition()==DOWNSIDE )	
				 {
					 KeyS_actEN_LidOpen=0;KeyS_actOK_LidOpen=1;OpStep=STANDBY;UART_SendStr("ready to test\n>>");//OUTPUT7=0;
				 }
			}
			else	OpStep=OPEN_DOOR;
			break;
			
		case OPEN_DOOR:
			if(PB_START!=PRESS)	
			{
				 OUTPUT7=0;OpenTheDoor();delay_ms(100);
				 if(DoorPosition()==UPSIDE) OpStep=DUT_GO_OUTSIDE;
				//else OpenTheDoor();	
			}
			else OpStep=CLOSE_DOOR;
			break;
			
		case DUT_GO_OUTSIDE:		
			if(PB_START!=PRESS)	
			{	
				 OUTPUT7=0;DutGoOutside();delay_ms(100);
				 if(DutPosition()==OUTSIDE) OpStep=STANDBY;
				 else DutGoOutside();
			}
			else	OpStep=DUT_GO_INSIDE;
			break;
			
		default:
			break;		
	}
}

static UCHAR DutPosition(void)
{
	if		 (INPUT2==1 && INPUT3==0) return INSIDE;
	else if(INPUT2==0 && INPUT3==1) return OUTSIDE;	
	else {return UNDEFINE;}
}

static UCHAR DoorPosition(void)
{
	if		 (INPUT9==0 && INPUT10==1) return UPSIDE;
	else if(INPUT9==1 && INPUT10==0) return DOWNSIDE;
	else {return UNDEFINE;}	
}

