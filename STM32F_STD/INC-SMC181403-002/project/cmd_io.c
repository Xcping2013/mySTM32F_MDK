#include <stdlib.h>
#include "main.h"
#include "bits.h"
#include <string.h>

#include "Commands.h"
#include "IO.h"
#include "UART.h"

uint8_t  CommmandLine_in(char *Commands)
{
	if( strcmp(Commands,"inputs")==0  ) 
	{
		u8 i; u8 readData[18];
		for(i=0; i<18; i++)
		{
			readData[i]=GetInput(i+1)	? '1':'0';
		}
		UART_Printf("INPUTS<%.18s>\n",readData);
		return 1;
		
	}
	
	else if( strncmp(Commands,"int[",4)==0  ) 
	{	
		char *p = NULL;
		char *s = &Commands[4];			
		u8 channel=strtol(s, &p, 10);	
		if(*p==']'&& *(p+1)=='\0'	&& channel>0 && channel<=18)
		{	
			UART_Printf("INT[%d]<%ld>\n",channel,GetInput(channel));
			return 1;
		}
		UART_SendStr("<NA>\n");return 1;	
	}
	else return 0;	
}
//
uint8_t  CommmandLine_out(char *Commands)
{
	if( strncmp(Commands,"outputs[",8)==0  ) 
	{		
		if(strlen(Commands)==23 && Commands[22]	==']' && Commands[23]=='\0')
		{
			u8 i;
			for(i=1;i<15;i++)
			{
				SetOutput(i, Commands[7+i]-'0');
			}
			UART_Printf("<OK>\n");
			return 1;
		}
	}
	if( strncmp(Commands,"out[",4)==0  ) 
	{	
		char *p = NULL;
		char *s = &Commands[4];		
		u8 setval;		
		u8 channel=strtol(s, &p, 10);	
		if(*p==']'&& *(p+1)=='[' && channel>0 && channel<=14)	
		{
			s=NULL;
			setval=strtol(p+2, &s, 10);	
			if(*(s)==']'&& *(s+1)=='\0' )
			{
				SetOutput(channel, setval);
				UART_Printf("OUT[%d][%d]<OK>\n",channel,setval); 
				return 1;
			}			
		}	
		UART_SendStr("<NA>\n");return 1;
	}	
	else return 0;
}
//
uint8_t  CommmandLine_rgb(char *Commands)
{
	if( strncmp(Commands,"rgb[",4)==0  ) 
	{			
		u8 led=Commands[4];
		if(Commands[5]==']'&& Commands[6]=='\0'	)
		{
			if(led=='r')
			{
				RGB_R_ON();UART_SendStr("RGB[R]<OK>\n");return 1;
			}
			else if(led=='g')
			{
				RGB_G_ON();UART_SendStr("RGB[G]<OK>\n");return 1;
			}
			else if(led=='b')
			{
				RGB_B_ON();UART_SendStr("RGB[B]<OK>\n");return 1;
			}
			else if(led==' ')
			{
				RGB_OFF();UART_SendStr("RGB[ ]<OK>\n");return 1;
			}
		}
		UART_SendStr("<NA>\n");return 1;
	}	
	else return 0;
}
//
