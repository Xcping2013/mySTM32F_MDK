/**
  ******************************************************************************
  * @file    main.h 
  * @author  IPC Rennes
  * @version V2.1
  * @date    October 15, 2013
  * @brief   Header for main.c module
  * @note    (C) COPYRIGHT 2013 STMicroelectronics 
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "cspin.h"
#include "cspin_config.h"
#include "clock.h"

/** @defgroup cSPIN FW library interface
  *
  * {
  */

/* Exported variables */
  extern cSPIN_RegsStruct_TypeDef cSPIN__RegsStructArray[NUMBER_OF_SLAVES];
  extern double MAX_SPEED[NUMBER_OF_SLAVES];
  extern uint8_t daisy_chain;
  extern uint8_t number_of_slaves;  

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


/** @} */  
  
#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/
