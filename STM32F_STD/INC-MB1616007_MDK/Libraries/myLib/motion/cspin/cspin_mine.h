#ifndef __CSPIN_MINE_H
#define __CSPIN_MINE_H

/* Includes ------------------------------------------------------------------*/
#include "mcu_gpio.h"
#include "cspin.h"


//No Daisy Chain Mode
#define cSPIN_SPI					SPI2
#define cSPIN_SCK_Pin			GPIO_Pin_13
#define cSPIN_SCK_Port		GPIOB

#define cSPIN_MISO_Pin		GPIO_Pin_14
#define cSPIN_MISO_Port		GPIOB

#define cSPIN_MOSI_Pin		GPIO_Pin_15
#define cSPIN_MOSI_Port		GPIOB

#define cSPIN_PWM1				PC6
#define cSPIN_PWM2				PC7
#define cSPIN_PWM3_Pin		PC8
#define cSPIN_PWM3_Port		GPIOC

#define cSPIN_nSS1_Pin		GPIO_Pin_12
#define cSPIN_nSS1_Port		GPIOB
#define cSPIN_BUSY1_Pin		GPIO_Pin_8
#define cSPIN_BUSY1_Port	GPIOD
#define cSPIN_FLAG1_Pin		GPIO_Pin_9
#define cSPIN_FLAG1_Port	GPIOD
#define cSPIN_STBY_RESET1_Pin    GPIO_Pin_10
#define cSPIN_STBY_RESET1_Port   GPIOD
#define cSPIN_SW1_Pin    GPIO_Pin_11
#define cSPIN_SW1_Port   GPIOD

#define cSPIN_nSS2_Pin		GPIO_Pin_2
#define cSPIN_nSS2_Port		GPIOE
#define cSPIN_BUSY2_Pin		GPIO_Pin_5
#define cSPIN_BUSY2_Port	GPIOE
#define cSPIN_FLAG2_Pin		GPIO_Pin_4
#define cSPIN_FLAG2_Port	GPIOE
#define cSPIN_STBY_RESET2_Pin    GPIO_Pin_3
#define cSPIN_STBY_RESET2_Port   GPIOE
#define cSPIN_SW2_Pin    GPIO_Pin_0
#define cSPIN_SW2_Port   GPIOB


#define cSPIN_nSS3_Pin		GPIO_Pin_7
#define cSPIN_nSS3_Port		GPIO7
#define cSPIN_BUSY3_Pin		GPIO_Pin_4
#define cSPIN_BUSY3_Port	GPIOD

#define cSPIN_FLAG3_Pin		GPIO_Pin_5
#define cSPIN_FLAG3_Port	GPIOD

#define cSPIN_STBY_RESET3_Pin    GPIO_Pin_6
#define cSPIN_STBY_RESET3_Port   GPIOD

#define TIM_PWM                 TIM3
	
#define cSPIN_PERIPHERAL_CLKs_APB1	(RCC_APB1Periph_SPI2|RCC_APB1Periph_TIM3)
#define cSPIN_PERIPHERAL_CLKs_APB2	(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO)


#endif 

/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/
