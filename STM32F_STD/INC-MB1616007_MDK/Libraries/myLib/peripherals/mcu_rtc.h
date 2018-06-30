
#ifndef __MCU_RTC_H
#define __MCU_RTC_H
#include "sys.h"

typedef enum  
{
		Sec_Irq = 0,
		Alr_Irq = 1,
		Ow_Irq = 2,
}Rtc_IrqType;

typedef void (*rtc_irq_handler)(uint32_t id, Rtc_IrqType type);



/*
	1.提供一个32位的循环计数,每秒加1.
	2.一个中断源，三个中断事件
	3.支持一个简易时钟
	4.支持一个闹钟
    
*/

typedef struct  
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint32_t count;
	
	int (*init)(uint8_t clock_source);//1:LSE;0:LSI如果使用外部晶振
	
	
	void (*set_counter)(uint32_t count);
  uint32_t (*get_counter)(void);
	
	void (*set_alarm)(uint32_t count);

	void (*sec_it)(void);
	void (*alarm_it)(void);
	void (*overflow_it)(void);
	
	void (*it_en)(uint8_t, FunctionalState state);
	
}Rtc_DrvTypeDef;

extern Rtc_DrvTypeDef 	rtc;

void sec_event(void);
void set_clock(uint8_t h,uint8_t m,uint8_t s);
int  Rtc_init(uint8_t clock_source);
void Rtc_set_counter(uint32_t count);
uint32_t Rtc_get_counter(void);
void Rtc_set_alarm(uint32_t count);

void Rtc_interrupt_en(uint8_t type, FunctionalState state);

#endif




