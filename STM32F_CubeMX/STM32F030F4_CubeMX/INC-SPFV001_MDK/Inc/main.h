/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "delay.h"
#include "tm1638.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define LED_START_Pin GPIO_PIN_0
#define LED_START_GPIO_Port GPIOA
#define TM1638_DIO_Pin GPIO_PIN_1
#define TM1638_DIO_GPIO_Port GPIOA
#define TM1638_SCK_Pin GPIO_PIN_2
#define TM1638_SCK_GPIO_Port GPIOA
#define TM1638_STB_Pin GPIO_PIN_3
#define TM1638_STB_GPIO_Port GPIOA
#define BEEPER_Pin GPIO_PIN_5
#define BEEPER_GPIO_Port GPIOA
#define SSR_CON_Pin GPIO_PIN_6
#define SSR_CON_GPIO_Port GPIOA
#define ALM_DIN_Pin GPIO_PIN_7
#define ALM_DIN_GPIO_Port GPIOA
#define LED_SYS_RUN_Pin GPIO_PIN_14
#define LED_SYS_RUN_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
#define BEEP_ON() 				HAL_GPIO_WritePin(BEEPER_GPIO_Port,BEEPER_Pin,GPIO_PIN_SET)
#define BEEP_OFF() 				HAL_GPIO_WritePin(BEEPER_GPIO_Port,BEEPER_Pin,GPIO_PIN_RESET)
#define BEEP_TOGGLE()			HAL_GPIO_TogglePin(BEEPER_GPIO_Port,BEEPER_Pin)

#define RUN_LED_ON() 	  	HAL_GPIO_WritePin(LED_START_GPIO_Port,LED_START_Pin,GPIO_PIN_SET)
#define RUN_LED_OFF() 		HAL_GPIO_WritePin(LED_START_GPIO_Port,LED_START_Pin,GPIO_PIN_RESET)
#define RUN_LED_TOGGLE() 	HAL_GPIO_TogglePin(LED_START_GPIO_Port,LED_START_Pin)

#define SYS_LED_ON() 	  	HAL_GPIO_WritePin(LED_SYS_RUN_GPIO_Port,LED_SYS_RUN_Pin,GPIO_PIN_SET)
#define SYS_LED_OFF() 		HAL_GPIO_WritePin(LED_SYS_RUN_GPIO_Port,LED_SYS_RUN_Pin,GPIO_PIN_RESET)
#define	SYS_LED_BLINK()		HAL_GPIO_TogglePin(LED_SYS_RUN_GPIO_Port,LED_SYS_RUN_Pin)
	
#define SSR_ON() 					HAL_GPIO_WritePin(SSR_CON_GPIO_Port,SSR_CON_Pin,GPIO_PIN_SET)
#define SSR_OFF()					HAL_GPIO_WritePin(SSR_CON_GPIO_Port,SSR_CON_Pin,GPIO_PIN_RESET)
#define SSR_TOGGLE()			HAL_GPIO_TogglePin(SSR_CON_GPIO_Port,SSR_CON_Pin)

#define	KEY_START		1
#define	KEY_SET			2
#define	KEY_UP			3
#define	KEY_DOWN		4
#define	KEY_BRT			5
#define	KEY_NONE		0

extern uint16_t BeeperTimeCnt;
extern uint8_t  segDigit4_BlinkBit;
extern uint32_t CurrentTimeVaule;
void SYSTICK_Task1Ms(void);
/* USER CODE END Private defines */

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
