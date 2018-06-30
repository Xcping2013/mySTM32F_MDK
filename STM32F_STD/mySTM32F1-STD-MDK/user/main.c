#include "sys.h"
#include "delay.h"
#include "usart.h"	 
#include "usmart.h"	

#include "led.h"
#include "input.h"	
#include "output.h"
#include "at24cxx.h" 
#include "motor_motion.h"

#include <stdlib.h>

int main(void)
{	 
	u32 BlinkDelay;
	u32 LimitDelay;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	InitSysTick();	 
	uart_init(115200);	 
	
 	LED_Init();			    														
	input_init();
	output_init();
	eeprom_init();
	
	motor_EN_PIN=at24cxx.readU8(at24c256,ADDR_EN_PIN);
	motor_EN_VALUE=at24cxx.readU8(at24c256,ADDR_EN_VALUE);

	rt_hw_tmc429_init();
	
	usmart_dev.init(SystemCoreClock/1000000);		
	
	printf("fixture init ok\r\n>>");
	
	BlinkDelay=GetSysTimer();
	LimitDelay=GetSysTimer();
	while(1)
	{
  	if(abs((int)(GetSysTimer()-BlinkDelay))>350)
  	{
  		pinToggle(&PD7);
  		BlinkDelay=GetSysTimer();
		} 
		usmart_dev.scan();
		if(abs((int)(GetSysTimer()-LimitDelay))>15)
  	{
			motor_en_get();
  		motor_limit_process();
  		LimitDelay=GetSysTimer();
		} 	
	}
}


