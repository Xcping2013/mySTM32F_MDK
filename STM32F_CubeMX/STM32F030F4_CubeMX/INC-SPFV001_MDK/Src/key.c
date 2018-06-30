#include "main.h"
#include "stm32f0xx_hal.h"
#include "gpio.h"

#define KEY_SHAKE_TIME				5

#define SHOW_TIME							1
#define	SET_TIME							2
#define	RUN_TIME							3
#define	BEEP_TIME							500

#define	MAX_TTIME							5999*1000
#define	DEFAULT_TTIME					330*1000	//max=5999*1000			10*1000//

uint8_t  TimerStart=0;
uint8_t  segDigit4_BlinkBit=0;
uint8_t  segDigit4_DisMode=SHOW_TIME;
uint32_t Factor=1;
uint32_t CurrentTimeVaule=DEFAULT_TTIME;
int32_t  SetTimeVaule=DEFAULT_TTIME;
uint32_t AddFactor[4]={1*1000,10*1000,60*1000,600*1000};
uint8_t  KeyValue=0xff;
uint8_t  KeySelfLock[5]={0,0,0,0,0}; 
uint8_t  KeyTimeCnt[5]={0,0,0,0,0}; 

uint16_t BeeperTimeCnt=0;
uint8_t  DisValue[5]={0,0,3,5,0}; 

void SYSTICK_Task1Ms(void)
{
	static u8 ssrDelay;
	if(TimerStart)					
	{
		if(CurrentTimeVaule==0)
		{
			BEEP_ON();
			BeeperTimeCnt=BEEP_TIME;
			CurrentTimeVaule=SetTimeVaule;
			TimerStart=0;RUN_LED_OFF();segDigit4_DisMode=SHOW_TIME;	
		}
		else CurrentTimeVaule-=1;
		
		ssrDelay++;
		if(ssrDelay>100)
		{
			ssrDelay=0;
			if(HAL_GPIO_ReadPin(ALM_DIN_GPIO_Port,ALM_DIN_Pin))
			{
				SSR_ON();
			}
			else SSR_OFF();
		}
	}
	else SSR_OFF();
	
	if(BeeperTimeCnt==0) BEEP_OFF();	else BeeperTimeCnt--;
	

}
//
void KeyScan(uint8_t PinStatus)
{ 
	switch(PinStatus)
	{
		case KEY_START:
			if(KeySelfLock[KEY_START]==0 )
			{
				if((KeyTimeCnt[KEY_START]++)	>KEY_SHAKE_TIME)
				{
					KeySelfLock[KEY_START]=1; 
					KeyTimeCnt[KEY_START]=0; 
					KeyValue=KEY_START;
				}
			}
			break;
	  
		case KEY_SET:
			if(KeySelfLock[KEY_SET]==0 )
			{
				if((KeyTimeCnt[KEY_SET]++)	>KEY_SHAKE_TIME)
				{
					KeySelfLock[KEY_SET]=1; 
					KeyTimeCnt[KEY_SET]=0; 
					KeyValue=KEY_SET;
				}
			}
			break;

		case KEY_UP:
			if(KeySelfLock[KEY_UP]==0 )
			{
				if((KeyTimeCnt[KEY_UP]++)	>KEY_SHAKE_TIME)
				{
					KeySelfLock[KEY_UP]=1; 
					KeyTimeCnt[KEY_UP]=0; 
					KeyValue=KEY_UP;
				}
			}
			break;	
					
		case KEY_DOWN:
			if(KeySelfLock[KEY_DOWN]==0 )
			{
				if((KeyTimeCnt[KEY_DOWN]++)	>KEY_SHAKE_TIME)
				{
					KeySelfLock[KEY_DOWN]=1; 
					KeyTimeCnt[KEY_DOWN]=0; 
					KeyValue=KEY_DOWN;
				}
			}
			break;	
					
		default:
				KeySelfLock[KEY_START]=0; 
				KeyTimeCnt[KEY_START]=0; 	
			
				KeySelfLock[KEY_SET]=0; 
				KeyTimeCnt[KEY_SET]=0; 	
			
				KeySelfLock[KEY_UP]=0; 
				KeyTimeCnt[KEY_UP]=0; 
			
				KeySelfLock[KEY_DOWN]=0; 
				KeyTimeCnt[KEY_DOWN]=0; 
			break;

	}
}
void runTime(void)
{
	if(KeyValue==KEY_START)
	{	
		TimerStart=0;	RUN_LED_OFF(); segDigit4_DisMode=SHOW_TIME;	
		CurrentTimeVaule=SetTimeVaule;
		KeyValue=KEY_NONE;
		BeeperTimeCnt=BEEP_TIME;
		BEEP_ON();
	}
}
//
void showTime(void)
{
	static u8 tempBRT=0;
	switch(KeyValue)
	{
		case KEY_START:			
		
			TimerStart=1;	RUN_LED_ON();	segDigit4_DisMode=RUN_TIME;	
			KeyValue=KEY_NONE;
			break;
			
		case KEY_SET:			
			
		  segDigit4_DisMode=SET_TIME;		
			Factor=AddFactor[0];segDigit4_BlinkBit=1;	
			TM1638_writeData(0, SEG_BIT1);		
//			DisValue[4] = Display_Time[SetTimeVaule/AddFactor[3]];			SetTimeVaule %= AddFactor[3];
//			DisValue[3] = Display_Time[SetTimeVaule/AddFactor[2]];			SetTimeVaule %= AddFactor[2];
//			DisValue[2] = Display_Time[SetTimeVaule/AddFactor[1]];			SetTimeVaule %= AddFactor[1];
//			DisValue[1] = Display_Time[SetTimeVaule/AddFactor[0]];
			KeyValue=KEY_NONE;
			break;
		
		case KEY_DOWN:
			if(tempBRT<7)	tempBRT++;
			else tempBRT=0;
			TM1638_setBrightness(tempBRT);
			KeyValue=KEY_NONE;
			break;
		
		default:
			KeyValue=KEY_NONE;
			break;
	}
}
//
void setTime(void)
{
	static uint8_t Fnum=1;
	switch(KeyValue)
	{
		case KEY_START:	
			Fnum++;			
			if(Fnum<5)	
			{
				Factor=AddFactor[Fnum-1];segDigit4_BlinkBit=Fnum;
			} 
			else 
			{
				Fnum=1;	Factor=AddFactor[0];segDigit4_BlinkBit=1;
			}
			KeyValue=KEY_NONE;
			break;
			
		case KEY_SET:	
			
			segDigit4_DisMode=SHOW_TIME;
			CurrentTimeVaule=SetTimeVaule;
			Fnum=1;	Factor=AddFactor[0];segDigit4_BlinkBit=0;
	
			KeyValue=KEY_NONE;
			break;
		
//		case KEY_UP:
//			
//			if(segDigit4_BlinkBit!=2)
//			{
//				if(DisValue[segDigit4_BlinkBit]==9) DisValue[segDigit4_BlinkBit]=0;
//				else DisValue[segDigit4_BlinkBit]+=1;		
//			}
//			else
//			{
//				if(DisValue[segDigit4_BlinkBit]==5) DisValue[segDigit4_BlinkBit]=0;
//				else DisValue[segDigit4_BlinkBit]+=1;
//				
//			}
//			SetTimeVaule=(DisValue[4]*600*+DisValue[3]*60+DisValue[2]*10+DisValue[1])*1000;
//			CurrentTimeVaule=SetTimeVaule;
//			KeyValue=KEY_NONE;
//			break;

//		case KEY_DOWN:
//			
//			if(segDigit4_BlinkBit!=2)
//			{
//				if(DisValue[segDigit4_BlinkBit]==0) DisValue[segDigit4_BlinkBit]=9;
//				else 		DisValue[segDigit4_BlinkBit]-=1;
//			}
//			else
//			{
//				if(DisValue[segDigit4_BlinkBit]==0) DisValue[segDigit4_BlinkBit]=5;
//				else DisValue[segDigit4_BlinkBit]-=1;
//			}
//			SetTimeVaule=(DisValue[4]*600*+DisValue[3]*60+DisValue[2]*10+DisValue[1])*1000;
//			CurrentTimeVaule=SetTimeVaule;	
//			KeyValue=KEY_NONE;
//			break;			
		case KEY_UP:
			SetTimeVaule=SetTimeVaule+Factor;
			if(SetTimeVaule>MAX_TTIME) SetTimeVaule=MAX_TTIME;
			CurrentTimeVaule=SetTimeVaule;
			KeyValue=KEY_NONE;
			break;
		
		case KEY_DOWN:
			SetTimeVaule=SetTimeVaule-Factor;	
			if(SetTimeVaule<0) SetTimeVaule=0;
			CurrentTimeVaule=SetTimeVaule;
			KeyValue=KEY_NONE;
			break;
					
		default:
			KeyValue=KEY_NONE;
			break;
	}
}
//
void KeyProcess(void)
{
	switch (segDigit4_DisMode)
	{
		case SHOW_TIME:
				showTime();
				break;
		case RUN_TIME:
				runTime();
				break;
		case SET_TIME:
				setTime();
				break;
		default:
				break;
	}	
}
//

