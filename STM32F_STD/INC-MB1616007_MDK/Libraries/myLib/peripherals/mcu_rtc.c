
/* Includes ------------------------------------------------------------------*/
#include "mcu_rtc.h"
#define RTC_CFG_FLAG 0XA6A6

#define RTC_DEBUG (0)
#if RTC_DEBUG
#include "uart.h"
#define DEBUG printf
#else
#define DEBUG(...)
#endif

void (*irq_Sec_Irq_handler)(void);
void (*irq_Alr_Irq_handler)(void);
void (*irq_Ow_Irq_handler)(void);

static uint8_t Rtc_is_config(uint16_t configFlag);
static int Rtc_config(uint8_t flag);
static void Rtc_set_config_flag(uint16_t configFlag);
static void Rtc_nvic(FunctionalState state);

void sec_event(void)
{
		rtc.count ++;
		rtc.count %=3600*24;
		rtc.sec  = (rtc.count % 60);
		rtc.min  = (rtc.count % 3600)/ 60;
		rtc.hour = (rtc.count / 3600);
}
void set_clock(uint8_t h,uint8_t m,uint8_t s)
{
		rtc.count = h*3600 + m*60 + s;        
}

int Rtc_init(uint8_t clock_source)
{
    int ret = EOK;
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);
    /* Reset Backup Domain */
		//BKP_DeInit();
    if(Rtc_is_config(RTC_CFG_FLAG) == 0)
    {			
        if(Rtc_config(clock_source) != EOK)
        {
            Rtc_config(0);
            ret = EPARA;
        }
    
        Rtc_set_config_flag(RTC_CFG_FLAG);
    }
	else
	{
		
		/* Check if the Power On Reset flag is set */
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
			DEBUG("\r\n\n Power On Reset occurred....");
		}
		/* Check if the Pin Reset flag is set */
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			DEBUG("\r\n\n External Reset occurred....");
		}
		
		DEBUG("\r\n No need to configure RTC....");
		DEBUG("\r\n step 0....");
		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();
		DEBUG("\r\n step 1....");
		
		/* Enable the RTC Second */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		DEBUG("\r\n step 2....");
		
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
        
		DEBUG("\r\n step 3....");
	}
	
	
	irq_Sec_Irq_handler=rtc.sec_it;
	irq_Alr_Irq_handler=rtc.alarm_it;
	irq_Ow_Irq_handler=rtc.overflow_it;
	
	Rtc_nvic(ENABLE);
	Rtc_get_counter();
	
  return ret;
}
int Rtc_config(uint8_t flag)
{

    int ret;
    uint32_t i=0;
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
    BKP_DeInit();
    
    if(flag == 1)
    {
        /* Enable LSE */
        RCC_LSEConfig(RCC_LSE_ON);
        /* Wait till LSE is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        {
            i++;
            if(i >= 0x1fffff)
            {
                ret = ETIMEOUT;
                return ret;
            }
        }            
        /* Select LSE as RTC Clock Source */
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    }
    else
    {
        RCC_LSICmd(ENABLE);
        /* Wait till LSI is ready */
        while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

        /* Enable RTC Clock */
        RCC_RTCCLKCmd(ENABLE);

    }

    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    if(flag == 1)
    {
        /* Set RTC prescaler: set RTC period to 1sec */
        RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
    }
    else
    {
        /* Set RTC prescaler: set RTC period to 1sec */
        RTC_SetPrescaler(39999); /* RTC period = RTCCLK/RTC_PR = (40 KHz)/(39999+1) */

    }

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
       
    
    Rtc_nvic(ENABLE);

    return EOK;
}
uint8_t Rtc_is_config(uint16_t configFlag)
{
    return (BKP_ReadBackupRegister(BKP_DR1) == configFlag);
}
void Rtc_set_config_flag(uint16_t configFlag)
{
    BKP_WriteBackupRegister(BKP_DR1, configFlag);
}
void Rtc_set_counter(uint32_t count)
{
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Change the current time */
    RTC_SetCounter(count);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}
void Rtc_set_alarm(uint32_t count)
{
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Change the current time */
    RTC_SetAlarm(count);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}


void Rtc_nvic(FunctionalState state)
{

    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    //	NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = state;
    NVIC_Init(&NVIC_InitStructure);
}

void Rtc_sec_interrupt(FunctionalState state)
{
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    if(state == ENABLE)
        RTC->CRH |= (1<<0);
    else
         RTC->CRH &= ~(1<<0);
}

void Rtc_alarm_interrupt(FunctionalState state)
{
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    if(state == ENABLE)
        RTC->CRH |= (1<<1);
    else
         RTC->CRH &= ~(1<<1);
}

void Rtc_overflow_interrupt(FunctionalState state)
{
     
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    if(state == ENABLE)
        RTC->CRH |= (1<<2);
    else
         RTC->CRH &= ~(1<<2);
}

//
void Rtc_interrupt_en(uint8_t type, FunctionalState state)
{
	switch(type)
	{
		case 1:	Rtc_sec_interrupt(state);
			break;
		case 2:	Rtc_alarm_interrupt(state);
			break;
		case 3:	Rtc_overflow_interrupt(state);
			break;		
	}	
}

uint32_t Rtc_get_counter()
{
    return RTC_GetCounter();
};
//	
void RTC_IRQHandler(void)
{
		if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
		{
				irq_Sec_Irq_handler();
				RTC_ClearITPendingBit(RTC_IT_SEC);
		}
		if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
		{
				irq_Alr_Irq_handler();
				RTC_ClearITPendingBit(RTC_IT_ALR);
		}
		if (RTC_GetITStatus(RTC_IT_OW) != RESET)
		{
				irq_Ow_Irq_handler();
				RTC_ClearITPendingBit(RTC_IT_OW);
		}
}
//
