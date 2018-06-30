#include <stdlib.h>
#include "main.h"
#include "bits.h"
#include <string.h>

#include "Commands.h"
#include "IO.h"
#include "UART.h"
#include "SysControl.h"
#include "lidopen.h"
#include "JABIL_LCR.h"

uint8_t  CommmandLine_Lidopen(char *Commands)
{
	if(strcmp("motor go home",Commands)==0)
  {
		HomeStep=1;MotorIsHomed=0;
		UART_SendStr("<OK>\n");
		return 1;
  }
	if(strcmp("button enable",Commands)==0)
	{
		if(ProID==LIDOPEN)
		{
			KeyS_actEN_LidOpen=1;KeyS_actOK_LidOpen=0;OUTPUT7=1;
		}
		else if(ProID==JABIL_LCR)
		{
			KeyS_actEN_LCR=1;KeyS_actOK_LCR=0;
		}
		UART_SendStr("<OK>\n");
		return 1;
	}
	if(strcmp("button disable",Commands)==0)
	{
		if(ProID==LIDOPEN)
		{
			KeyS_actEN_LidOpen=0;KeyS_actOK_LidOpen=0;OUTPUT7=0;
		}
		else if(ProID==JABIL_LCR)
		{
			KeyS_actEN_LCR=0;KeyS_actOK_LCR=0;
		}		
		UART_SendStr("<OK>\n");
		return 1;
	}
	if(strcmp("is button ok",Commands)==0)
	{
		switch (ProID)
		{
			case LIDOPEN:
				if(KeyS_actOK_LidOpen==1)
				{	
					UART_SendStr("BUTTON<OK>\n");
					return 1;
				}
				else 
				{	
					UART_SendStr("BUTTON<NO>\n");	
					return 1;
				}
			case JABIL_LCR:
				if(KeyS_actOK_LCR==1)
				{	
					UART_SendStr("BUTTON<OK>\n");
					return 1;
				}
				else 
				{	
					UART_SendStr("BUTTON<NO>\n");	
					return 1;
				}
		 }
	}
  return 0; 	
}
//
