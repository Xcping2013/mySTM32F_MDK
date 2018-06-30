/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

TIM_ClockConfigTypeDef sClockSourceConfig;
TIM_MasterConfigTypeDef sMasterConfig;
TIM_OC_InitTypeDef sConfigOC;

/* USER CODE BEGIN 0 */
#include <rtdevice.h>

#define TIM_RTT_DEBUG
#ifdef	TIM_RTT_DEBUG

#define TIM_TRACE         rt_kprintf
#else
#define TIM_TRACE(...)   
#endif
/* USER CODE END 0 */
TIM_HandleTypeDef htim3;

/* TIM3 init function */
void MX_TIM3_Init(void)
{
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 5;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	//htim3.Init.RepetitionCounter = 0;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    //_Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    //_Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    //_Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    //_Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM2;
  sConfigOC.Pulse = 2;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    //_Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim3);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(timHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspPostInit 0 */
		__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器3
		__HAL_AFIO_REMAP_TIM3_PARTIAL();		//TIM3通道引脚部分重映射使能
		__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
		
  /* USER CODE END TIM3_MspPostInit 0 */
  
    /**TIM3 GPIO Configuration    
    PB0     ------> TIM3_CH3 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM3_MspPostInit 1 */

  /* USER CODE END TIM3_MspPostInit 1 */
  }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
void tim_get_cfg(void)
{
	
	TIM_TRACE("\r\nhtim3.Init.Prescaler=%ld\r\n",htim3.Init.Prescaler);
	TIM_TRACE("htim3.Init.CounterMode=%ld\r\n",htim3.Init.CounterMode);	
	TIM_TRACE("htim3.Init.Period=%ld\r\n",htim3.Init.Period);
	TIM_TRACE("htim3.Init.ClockDivision=%ld\r\n",htim3.Init.ClockDivision);
	TIM_TRACE("htim3.Init.RepetitionCounter=%ld\r\n",htim3.Init.RepetitionCounter);
	TIM_TRACE("htim3.Init.AutoReloadPreload=%ld\r\n",htim3.Init.AutoReloadPreload);
	TIM_TRACE("htim3.Init.Channel=%ld\r\n",htim3.Channel);
	TIM_TRACE("htim3.Init.Lock=%ld\r\n",htim3.Lock);
	TIM_TRACE("htim3.Init.State=%ld\r\n",htim3.State);
			
	TIM_TRACE("\r\nsClockSourceConfig.ClockSource=%ld\r\n",sClockSourceConfig.ClockSource);
	TIM_TRACE("sClockSourceConfig.ClockPolarity=%ld\r\n",sClockSourceConfig.ClockPolarity);
	TIM_TRACE("sClockSourceConfig.ClockPrescaler=%ld\r\n",sClockSourceConfig.ClockPrescaler);
	TIM_TRACE("sClockSourceConfig.ClockFilter=%ld\r\n",sClockSourceConfig.ClockFilter);
	
	
	TIM_TRACE("\r\nsMasterConfig.MasterOutputTrigger=%ld\r\n",sMasterConfig.MasterOutputTrigger);
	TIM_TRACE("sMasterConfig.MasterSlaveMode=%ld\r\n",sMasterConfig.MasterSlaveMode);

	TIM_TRACE("\r\nsConfigOC.OCMode=%ld\r\n",sConfigOC.OCMode);
	TIM_TRACE("sConfigOC.Pulse=%ld\r\n",sConfigOC.Pulse);
	TIM_TRACE("sConfigOC.OCPolarity=%ld\r\n",sConfigOC.OCPolarity);
	TIM_TRACE("sConfigOC.OCNPolarity=%ld\r\n",sConfigOC.OCNPolarity);
	TIM_TRACE("sConfigOC.OCFastMode=%ld\r\n",sConfigOC.OCFastMode);
	TIM_TRACE("sConfigOC.OCIdleState=%ld\r\n",sConfigOC.OCIdleState);
	TIM_TRACE("sConfigOC.OCNIdleState=%ld\r\n",sConfigOC.OCNIdleState);	
}

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
