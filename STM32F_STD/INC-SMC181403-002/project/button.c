#include "main.h"
#include "Globals.h"

void RampInit_Button(void)
{
  uint8_t i;
	for(i=0; i<N_O_MOTORS; i++) 	//Ĭ�ϵ����Ҫ�������ã���ҪΪ�ٶȺͼ��ٶ�
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

