#include "pcm1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PCM1_DEBUG

#ifdef  PCM1_DEBUG

#define PCM1_TRACE         printf
#else
#define PCM1_TRACE(...)    
#endif

GPIO_TypeDef *RELAY_port[10]={GPIOA,GPIOA,GPIOA,GPIOA,GPIOB,GPIOA,GPIOA,GPIOA,GPIOA,GPIOA};
uint16_t	RELAY_pin[10]=
{
	K1_GPIO_Pin,
	K2_GPIO_Pin,
	K2_GPIO_Pin,
	K4_GPIO_Pin,
	K5_GPIO_Pin,
	K6_GPIO_Pin,
	K7_GPIO_Pin,
	K7_GPIO_Pin,
	K9_GPIO_Pin,
	K10_GPIO_Pin,
};
void relay_close(uint8_t number)
{
	if(0<number && number<11)
	{
		HAL_GPIO_WritePin(RELAY_port[number-1], RELAY_pin[number-1], GPIO_PIN_RESET);
		PCM1_TRACE("ok");
	}
}

void relay_open(uint8_t number)
{
	if(0<number && number<11)
	{
		HAL_GPIO_WritePin(RELAY_port[number-1], RELAY_pin[number-1], GPIO_PIN_SET);
		PCM1_TRACE("ok");
	}
}
