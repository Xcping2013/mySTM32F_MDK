  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f0xx.h"

#define SYS_LED_Pin 					GPIO_Pin_1
#define SYS_LED_GPIO_Port 		GPIOF
#define BME280_CS_Pin 				GPIO_Pin_1
#define BME280_CS_GPIO_Port 	GPIOB
#define HT2201_SDA_Pin 				GPIO_Pin_13
#define HT2201_SDA_GPIO_Port	GPIOA
#define HT2201_SCL_Pin 				GPIO_Pin_14
#define HT2201_SCL_GPIO_Port 	GPIOA

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
