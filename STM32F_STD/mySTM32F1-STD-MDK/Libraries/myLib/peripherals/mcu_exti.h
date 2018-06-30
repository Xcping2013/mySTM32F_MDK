
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
}exti_t;
	
void exti_init(exti_t *dev);
void exti_nvic_init(exti_t *dev);

#endif
