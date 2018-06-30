
#include "time3_clk_tmc429.h"

TIM_HandleTypeDef 	TIM3_Handler;
TIM_OC_InitTypeDef 	TIM3_CH3Handler;		

void TIM3_PWM_Init(void)
{  
		TIM_MasterConfigTypeDef sMasterConfig;
	
    TIM3_Handler.Instance=TIM3;          					   
    TIM3_Handler.Init.Prescaler=0;       					 
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;
    TIM3_Handler.Init.Period=5;          					
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
		TIM3_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_PWM_Init(&TIM3_Handler);    
  
		sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
		HAL_TIMEx_MasterConfigSynchronization(&TIM3_Handler, &sMasterConfig);
  
    TIM3_CH3Handler.OCMode=TIM_OCMODE_PWM2; 			
    TIM3_CH3Handler.Pulse=2;            				   
    TIM3_CH3Handler.OCPolarity=TIM_OCPOLARITY_HIGH; 
		TIM3_CH3Handler.OCFastMode = TIM_OCFAST_ENABLE;
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH3Handler,TIM_CHANNEL_3);
	
//		HAL_TIM_MspPostInit(&TIM3_Handler);
		
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_3);   
}
//
