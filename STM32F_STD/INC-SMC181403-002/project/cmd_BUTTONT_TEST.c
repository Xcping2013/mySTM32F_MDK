#include <stdlib.h>
#include "main.h"
#include "bits.h"
#include <string.h>

#include "Commands.h"
#include "IO.h"
#include "UART.h"
#include "SysControl.h"

//
uint8_t  CommmandLine_Button(char *Commands)
{
	if(strcmp("is homed",Commands)==0)
  {
		if(MotorIsHomed) 	UART_SendStr("HOME<OK>\n");
		else 							UART_SendStr("HOME<NO>\n");
		return 1;
  }	
	if(strcmp("motor reset",Commands)==0)
  {
		UART_SendStr(">>");
		MotorHoming();
		//UART_SendStr("<OK>\n");
		return 1;
  }
	if(strcmp("is start",Commands)==0)
  {
		if(KeySTART_ON) 	UART_SendStr("START<OK>\n");
		else 									UART_SendStr("START<NO>\n");
		return 1;
  }	
	else if(strcmp("test over",Commands)==0)
  {
		KeySTART_ON=0;
		OUTPUT1=0;
		UART_SendStr("<OK>\n");
		return 1;
  }
	else return 0;
}
