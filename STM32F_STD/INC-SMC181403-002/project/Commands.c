/**
  \file 		Commands.c
  \author 	Xcping2016
  \version 	1.00
*/

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "bits.h"
#include "main.h"
#include "Commands.h"
#include "Globals.h"
#include "UART.h"
#include "SysTick.h"

#include "Eeprom.h"
#include "SysControl.h"

#include "IO.h"
#include "lidopen.h"
#include "JABIL_LCR.h"

static void 		MakeLowercase( char *str);
static void 		PrintCommandlist(void);
static uint8_t  CommmandLine_help(char *Commands);

static void MakeLowercase( char *str)		
{
	while(*str!='\0')
	{
			if( ((*str)>='A')&&((*str)<='Z') )
			{
					*str=*str+32;
			}
			str++;
	}
}

//
uint8_t  ProcessCommand(char  *Commands)
{
	MakeLowercase(Commands);	

	if		  (CommmandLine_help(Commands)==1)								return 1;
	
	else	if(CommmandLine_in(Commands)==1)									return 1;
	else	if(CommmandLine_out(Commands)==1)									return 1;
	else	if(CommmandLine_rgb(Commands)==1)									return 1;
	
	else	if(CommmandLine_baud(Commands)==1)								return 1;
	else	if(CommmandLine_sv(Commands)==1)									return 1;
	else	if(CommmandLine_hv(Commands)==1)									return 1;
	else	if(CommmandLine_fsn(Commands)==1)									return 1;	
	else  if(CommmandLine_rom(Commands)==1)									return 1;	
	else	if(CommmandLine_RomPage(Commands)==1)							return 1;		

	else	if(CommmandLine_speed(Commands)==1)								return 1;
	else	if(CommmandLine_acc(Commands)==1)									return 1;
	else	if(CommmandLine_rotate(Commands)==1)							return 1;		
	else	if(CommmandLine_move(Commands)==1)								return 1;
	
	else	if(CommmandLine_pos(Commands)==1)									return 1;
	else	if(CommmandLine_limit(Commands)==1)								return 1;
	else	if(CommmandLine_reach(Commands)==1)								return 1;	
	
	else	if(CommmandLine_Button(Commands)==1)							return 1;
	else	if(CommmandLine_Lidopen(Commands)==1)							return 1;
	else	if(CommmandLine_IAC_LCR(Commands)==1)							return 1;
	else	if(CommmandLine_JABIL_LCR(Commands)==1)						return 1;	
//	else if(strcmp("reset",		Commands)==0)								{		ResetCPU(TRUE);																														return 1;		}		

//board iO test
	else if(strcmp("io test",							Commands)==0)	{		Board_debugFlag=1;ProID=PROJECT_NONE;										UART_SendStr("OK\n");					return 1;		}		
//Project switch	
	else if(strcmp("type[button online]",	Commands)==0)	{		AT24CXX_WriteOneByte(ADDR_PROJECT_ID,BUTTON_ONLINE);	UART_SendStr("OK\n");					return 1;		}		
	else if(strcmp("type[button offline]",Commands)==0)	{		AT24CXX_WriteOneByte(ADDR_PROJECT_ID,BUTTON_OFFLINE);	UART_SendStr("OK\n");					return 1;		}		
	else if(strcmp("type[lidopen]",				Commands)==0)	{		AT24CXX_WriteOneByte(ADDR_PROJECT_ID,LIDOPEN);				UART_SendStr("OK\n");					return 1;		}	
	else if(strcmp("type[iac lcr]",				Commands)==0)	{		AT24CXX_WriteOneByte(ADDR_PROJECT_ID,IAC_LCR);				UART_SendStr("OK\n");					return 1;		}	
	else if(strcmp("type[jabil lcr]",		  Commands)==0)	{		AT24CXX_WriteOneByte(ADDR_PROJECT_ID,JABIL_LCR);			UART_SendStr("OK\n");					return 1;		}			

	else
	{
		return 0;
	}
}
/**				List the command list and discription--->succinctly、Unified specification	简洁明了，规范统一 发送小写，返回大写
**
**/
static void PrintCommandlist(void)
{	
	
	UART_SendStr("\nFirmware：20180426\n");
	UART_SendStr("\nproject select:\n");
	UART_SendStr("type[button online]----------no cylinder\n");	
	UART_SendStr("type[button offline]---------cylinder\n");
	UART_SendStr("type[lidopen]----------------lidopen\n");	
	UART_SendStr("type[iac lcr]----------------iac lcr\n");	
	UART_SendStr("type[jabil lcr]--------------jabil lcr\n");	
	
	UART_SendStr("\ninputs and outputs:\n");	
	UART_SendStr("int[1~18]--------------------get input\n");
	UART_SendStr("out[1~14][1 or 0]------------set output\n");
	UART_SendStr("rgb[r|g|b| ]-----------------set rgb led\n");
	UART_SendStr("inputs-----------------------get inputs\n");
	//UART_SendStr("outputs[101010101010101]-----Set outputs status\n");
	UART_SendStr("io test----------------------board debug\n");	
	
	UART_SendStr("\nsave and read data:\n");	
	UART_SendStr("fsn--------------------------read sn\n");	
	UART_SendStr("fsn[20180324]----------------save sn max to 10 chars\n");	
	UART_SendStr("rom[1~100]-------------------read floating number\n");
	UART_SendStr("rom[1~100][2018]-------------save floating number\n");
	UART_SendStr("read info 1~200--------------read max 64 bytes from position\n");
	UART_SendStr("save info 1~200 YYYYY--------write max 64 bytes to position\n");
	//UART_SendStr("baudRate[115200]------------Set the baudrate to 115200\n");
	if( ProID==BUTTON_ONLINE || ProID==BUTTON_OFFLINE || ProID==LIDOPEN )
	{
		UART_SendStr("\nread motion status:\n");	
		UART_SendStr("is left #0~2---REF[0~2]<1>----------read L limit\n");
		UART_SendStr("is right #0~2--REF[0~2]<1>----------read R limit\n");
		UART_SendStr("is limit-------LIMIT<L3R3L2R2L1R1>--read limit\n");
		UART_SendStr("is reach #0~2--REACH[0~2]<1>--------is targeted\n");
		UART_SendStr("is stop #0~2---STOP[0~2]<1>---------is stop\n");
		
		UART_SendStr("\nset motion parameters:\n");	
		UART_SendStr("0~2# velocity 0~2047----------------set the velocity to xx\n");
		UART_SendStr("all# velocity 0~2047----------------set all velocity to xx\n");
		UART_SendStr("0~2# acc 0~2047---------------------set the acc to n\n");	
		UART_SendStr("all# acc 0~2047---------------------set the acc to all\n");
		
		UART_SendStr("\nget motion parameters:\n");	
		UART_SendStr("velocity #0~2-----------------------get the velocity\n");
		UART_SendStr("velocity #all-----------------------get velocitys\n");	
		UART_SendStr("acc #0~2----------------------------get the acc\n");
		UART_SendStr("acc #all----------------------------get the accs\n");
		UART_SendStr("position #0~2-----------------------get current position\n");
		UART_SendStr("position #all-----------------------get current positions\n");
			
		UART_SendStr("\nmotor movement:\n");	
		UART_SendStr("n# rotate +/-xx--<OK>--------------Rotate Right or left\n");
		UART_SendStr("n# move to +/-xx-<OK>--------------#n to ABS POS -/-xx\n");
		UART_SendStr("n# move +/-xx----<OK>--------------#n to REL POS +/-xx\n");
		UART_SendStr("n# stop---------STOP[n]<OK>--------#n stop\n");
	}
	switch(ProID)
	{
		case LIDOPEN :
				{
					UART_SendStr("\nLidopen:\n");	
					UART_SendStr("button enable-----<OK>-------enable the button for next test\n");
					UART_SendStr("button disable----<OK>-------disable the button function\n");
					UART_SendStr("is button ok---<OK/NO>-------get button status\n");
					UART_SendStr("motor go home---<OK>---------start homing\n");
					UART_SendStr("is homed-------HOME<OK>------check motor homed status\n");		
					UART_SendStr("ready to test----------------door action ok\n");
				}
			break;
		case BUTTON_ONLINE:
		case BUTTON_OFFLINE:
				{
					UART_SendStr("\nButton:\n");	
					UART_SendStr("motor reset------<OK>--------start homing\n");
					UART_SendStr("test over--------<OK>--------enable key-start for next test\n"); 
					UART_SendStr("is homed-------HOME<OK>------check motor homed status\n");		
					UART_SendStr("is start-------START<OK>-----check key-start pressed status\n\n");	
				}
			break;
		case JABIL_LCR:
				{
					UART_SendStr("\nJABIL LCR:\n");	
					UART_SendStr("measure coil1 to coil2-------connect LCR between coil and coil2\n");
					UART_SendStr("measure coil1 to gnd---------connect LCR between coil and gnd\n");
					UART_SendStr("measure coil2 to gnd---------connect LCR between coi2 and gnd\n\n");
					UART_SendStr("enable start key-----<OK>----enable start key action\n");
					UART_SendStr("disable start key----<OK>----disable the start key\n");
					UART_SendStr("ready to test?---<YES/NO>----start Key act status\n");
					UART_SendStr("ready to test----------------start action ok\n");
				}
			break ;
		default : break;
	}
	UART_SendStr("\nPlease send command as above end with \\r\\n\n");
}
//
//-----------------------------------------------------------common	start
static uint8_t  CommmandLine_help(char *Commands)
{
	if(strcmp("help",	Commands)==0)				
	{		
		PrintCommandlist();								
		return 1;		
	}
	else return 0;
}
//
