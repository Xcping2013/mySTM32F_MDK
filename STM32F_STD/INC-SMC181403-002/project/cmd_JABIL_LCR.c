#include <string.h>

#include "bits.h"
#include "main.h"
#include "Commands.h"
#include "UART.h"
#include "JABIL_LCR.h"

uint8_t  CommmandLine_JABIL_LCR(char *Commands)
{
	if(strcmp("measure coil1 to coil2",Commands)==0)
	{
		U1_IN1_IN2=0;	U2_IN1_IN2=0;
		//GPIOD->BRR=BIT6|BIT7;					//relay1 off relay2 off
		UART_SendStr("<C1-C2>\n"); return 1;
	}
  if(strcmp("measure coil1 to gnd",Commands)==0)
	{
		U1_IN1_IN2=1;	U2_IN1_IN2=1;
		//GPIOD->BSRR=BIT6;	GPIOD->BSRR=BIT7;	//relay1 on relay on
		UART_SendStr("<C1-GND>\n"); return 1;
	}	
	if(strcmp("measure coil2 to gnd",Commands)==0)
	{
		U1_IN1_IN2=1;	U2_IN1_IN2=0;
		//GPIOD->BSRR=BIT6;	GPIOD->BRR=BIT7;	//relay1 on relay off
		UART_SendStr("<C2-GND>\n"); return 1;
	}
	if(strcmp("enable start key",Commands)==0)
	{
		KeyS_actEN_LCR=1;KeyS_actOK_LCR=0;	UART_SendStr("<OK>\n"); return 1;
	}
  if(strcmp("disable start key",Commands)==0)
	{
		KeyS_actEN_LCR=0;KeyS_actOK_LCR=0;	UART_SendStr("<OK>\n"); return 1;
	}	
	if(strcmp("ready to test?",Commands)==0)
	{
		if(KeyS_actOK_LCR==1)
		{	
			UART_SendStr("<YES>\n");
			return 1;
		}
		else 
		{	
			UART_SendStr("<NO>\n");	
			return 1;
		}
	}
	return 0;
}
//

