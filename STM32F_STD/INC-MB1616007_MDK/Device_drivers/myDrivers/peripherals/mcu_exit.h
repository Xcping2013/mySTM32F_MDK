
#ifndef __EXTI_H
#define __EXTI_H

#include "mcu_gpio.h"

typedef struct 
{
	uint8_t 						sub_priority;
	Gpio 								*pin;
	EXTITrigger_TypeDef trigger;
	FunctionalState 		en;
  void (*handler)(void);	
}exit_t;
	
void exit_init(exit_t *dev);
void exit_nvic_init(exit_t *dev);

#endif
