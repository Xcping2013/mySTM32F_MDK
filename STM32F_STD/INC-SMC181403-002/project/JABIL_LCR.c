#include <stdlib.h>
#include "main.h"
#include "IO.h"
#include "UART.h"
#include "systick.h"
#include "JABIL_LCR.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/*	GPIO：
		INPUT有上拉电阻，IN=0,INPUT=0,INT[N]=1
										 IN=1,INPUT=1,INT[N]=0
*/
/*	IO板资源：
		IN1	启动按键1														          OUT1 启动按键1		
		IN2	启动按键2														          OUT2 启动按键2
		IN3	DUT传感器在气缸里面  动位										  OUT3 DUT气缸进去 气管口在外面
		IN4	DUT传感器在气缸外面  始位										  OUT4 DUT气缸弹出 气管口在里面
			                                                
		IN5	顶针传感器在气缸外面 始位		JABIL_LCR 取消	  OUT5 顶针气缸按压 气管口在外面
		IN6 顶针传感器在气缸里面 动位		JABIL_LCR 取消	  OUT6 顶针气缸缩回 气管口在里面
		                                                  
		IN7 上下传感器在气缸上面 始位										  OUT7 上下气缸下压 气管口在上面
		IN8 上下传感器在气缸下面 动位										  OUT8 上下气缸上提 气管口在下面
*/
#define IN_ON					0
#define IN_OFF				1
#define OUT_ON				1
#define OUT_OFF				0
#define KEY_START1		INPUT1			
#define KEY_START2		INPUT2			

#define LED_START1		OUTPUT1
#define LED_START2		OUTPUT2

#define DutGoInside_LCR()		OUTPUT3=1;	OUTPUT4=0		
#define DutGoOutside_LCR()	OUTPUT3=0;	OUTPUT4=1		
#define CloseTheDoor_LCR()	OUTPUT7=1;	OUTPUT8=0		
#define OpenTheDoor_LCR()		OUTPUT7=0;	OUTPUT8=1		

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

uint8_t	OpStep_LCR=STANDBY;
uint8_t	KeyS_actOK_LCR=0;
uint8_t KeyS_actEN_LCR=0;
uint8_t KeyS_Delay=100;

static UCHAR DutPosition_JABIL_LCR(void);
static UCHAR DoorPosition_JABIL_LCR(void);

//
void SW_init_JABIL_LCR(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_12;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1|GPIO_Pin_12);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;									
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	U1_IN1_IN2=0;	U2_IN1_IN2=0;//默认切换状态为COIL1---COIL2
}
//
static UCHAR DutPosition_JABIL_LCR(void)
{
	if		 (INPUT3==IN_ON 	&& INPUT4==IN_OFF) return INSIDE;
	else if(INPUT3==IN_OFF 	&& INPUT4==IN_ON)  return OUTSIDE;	
	else {return UNDEFINE;}
}
static UCHAR DoorPosition_JABIL_LCR(void)
{
	if		 (INPUT7==IN_ON 	&& INPUT8==IN_OFF) return UPSIDE;
	else if(INPUT7==IN_OFF 	&& INPUT8==IN_ON)  return DOWNSIDE;
	else {return UNDEFINE;}	
}
//双启按键指示灯在收到上面机使能命令后便常亮,动作后灭指示是否可按
void KeyS_act_JABIL_LCR(void)																																		
{
	switch(OpStep_LCR)						
	{	
		case STANDBY:																																	

			if(DutPosition_JABIL_LCR()==OUTSIDE && DoorPosition_JABIL_LCR()==UPSIDE )		//DUT气缸在外面，门也正常打开		
			{
				static U32_T Delay=0;
				if(KEY_START1==IN_ON && KEY_START2==IN_OFF) 															//如果只按下按键1																	
				{			
					Delay=GetSysTimer();
					delay_ms(10);
					while(abs((int)(GetSysTimer()-Delay))<KeyS_Delay)															//需要在50ms内快速按下按键2，才会触发动作
					{
						if(KEY_START1==IN_ON && KEY_START2==IN_ON)	{	OpStep_LCR=DUT_GO_INSIDE;	break;	}				
					}
					if(OpStep_LCR==STANDBY)
					{					
						while(KEY_START1==IN_ON){UART_SendStr("start key press is invalid, please release and repress it\n>>");delay_ms(100);}//否则不触发动作，且需要等待松开按键1
					}													
				}
				if(KEY_START1==IN_OFF && KEY_START2==IN_ON) 															//如果只按下按键2																					
				{						
					Delay=GetSysTimer();
					delay_ms(10);
					while(abs((int)(GetSysTimer()-Delay))<KeyS_Delay)																//需要在50ms内快速按下按键1，才会触发动作
					{
						if(KEY_START1==IN_ON && KEY_START2==IN_ON)	{	OpStep_LCR=DUT_GO_INSIDE; break;	}
									
					}
					if(OpStep_LCR==STANDBY)
					{
						while(KEY_START2==IN_ON){UART_SendStr("start key press is invalid, please release it and repress\n>>");delay_ms(100);}																				 //否则不触发动作，且需要等待松开按键2
					}												
				}
				else
				{
					delay_ms(20);
					if(KEY_START1==IN_ON && KEY_START2==IN_ON)	{	OpStep_LCR=DUT_GO_INSIDE;	}	
				}
			}
			break;
			
		case DUT_GO_INSIDE:																									//保存双启，DUT进去
			if(KEY_START1==IN_ON && KEY_START2==IN_ON)	
			{	
				DutGoInside_LCR();
				delay_ms(100);
				if(DutPosition_JABIL_LCR()==INSIDE )	OpStep_LCR=CLOSE_DOOR;	
			}
			else	
			{
				OpStep_LCR=DUT_GO_OUTSIDE;
			}
			break;	
			
		case CLOSE_DOOR:																											//按下双启，关门
			if(KEY_START1==IN_ON && KEY_START2==IN_ON)	
			{	
				 CloseTheDoor_LCR();
				 delay_ms(100);
				 if(DoorPosition_JABIL_LCR()==DOWNSIDE )	
				 {
					 LED_START1=OUT_OFF; LED_START2=OUT_OFF;
					 KeyS_actEN_LCR=0;KeyS_actOK_LCR=1;OpStep_LCR=STANDBY;UART_SendStr("ready to test\n>>");
				 }
			}
			else																																//松开其中一个按键，开门
			{
				OpStep_LCR=OPEN_DOOR;
			}
			break;
			
		case OPEN_DOOR:
			if(KEY_START1==IN_OFF || KEY_START2==IN_OFF)												//只要松开双启的其中一个按键，门开
			{
				 OpenTheDoor_LCR();delay_ms(100);						
				 if(DoorPosition_JABIL_LCR()==UPSIDE) OpStep_LCR=DUT_GO_OUTSIDE;			//按下双启，关门
			}
			else OpStep_LCR=CLOSE_DOOR;
			break;
			
		case DUT_GO_OUTSIDE:								
			if(KEY_START1==IN_OFF || KEY_START2==IN_OFF)												//只要松开双启的其中一个按键，DUT气缸弹出
			{		
				 DutGoOutside_LCR();delay_ms(100);
				 if(DutPosition_JABIL_LCR()==OUTSIDE) OpStep_LCR=STANDBY;
				 else DutGoOutside_LCR();
			}
			else	OpStep_LCR=DUT_GO_INSIDE;																					//按下双启，DUT准备进去																									
			break;
			
		default:
			break;		
	}
}


