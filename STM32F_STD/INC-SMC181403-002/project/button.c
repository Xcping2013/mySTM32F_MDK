#include "main.h"
#include "Globals.h"

void RampInit_Button(void)
{
  uint8_t i;
	for(i=0; i<N_O_MOTORS; i++) 	//默认电机主要参数配置，主要为速度和加速度
	{																  
		MotorConfig[i].VMax=839;   //  3R/S  
		MotorConfig[i].AMax=1000;
		MotorConfig[i].PulseDiv=5;
		MotorConfig[i].RampDiv=8;	
		//MotorConfig[i].RampDiv=6;	
		SpeedChangedFlag[i]=TRUE;
	}
}
//

