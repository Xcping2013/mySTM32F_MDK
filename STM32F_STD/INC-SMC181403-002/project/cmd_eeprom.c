#include <stdlib.h>
#include "main.h"
#include <string.h>

#include "Commands.h"
#include "Eeprom.h"
#include "UART.h"

uint8_t  CommmandLine_sv(char *Commands)
{
  if(strcmp("sv",		Commands)==0)			
	{		
		PrintSoftVersionFromEEPROM();			
		return 1;		
	}
	else if(strncmp("sv[",	Commands,3)==0)	
	{		
		char *p = NULL;
		char *s = &Commands[3];			
		u8 sv=(uint8_t)strtol(s, &p, 10);	
		if(*p==']'&& *(p+1)=='\0')									
		{		
			WriteSWToEEPROM(sv);			
			return 1;		
		}
		UART_SendStr("<NA>\n");return 1;
	}	
	else return 0;
}
//
uint8_t  CommmandLine_hv(char *Commands)
{
	if(strcmp("hv",		Commands)==0)			
	{		
		PrintHardVersionFromEEPROM();			
		return 1;	
	}
	else if(strncmp("hv[",	Commands,3)==0)	
	{		
		char *p = NULL;
		char *s = &Commands[3];			
		u8 hv=(uint8_t )strtol(s, &p, 10);	
		if(*p==']'&& *(p+1)=='\0')									
		{		
			WriteHWToEEPROM(hv);		
			return 1;		
		}
		UART_SendStr("<NA>\n");return 1;
	}	
	else return 0;
}
//
uint8_t  CommmandLine_fsn(char *Commands)
{
	if(strcmp("fsn",		Commands)==0)			
	{		
		PrintFSNFromEEPROM();			
		return 1;	
	}
	else if(strncmp("fsn[",	Commands,4)==0)	
	{	
		uint8_t i;
		for(i=0;i<11;i++)
		{
			if(Commands[i+4]==']') {Commands[i+4]='\0';	break ;}
		}
		if(	i>0	&& i<11	&& Commands[i+4]=='\0'&& Commands[i+5]=='\0')			
		{		
			WriteFSNToEEPROM((uint8_t*)&Commands[4]);		
			return 1;		
		}
		UART_SendStr("<NA>\n");return 1;
	}	
	else return 0;
}
//
uint8_t  CommmandLine_baud(char *Commands)
{
	if(strncmp("baudrate[",	Commands,9)==0)	
	{	
		char *p = NULL;
		char *s = &Commands[9];			
		u32 baudrate=strtol(s, &p, 10);	
		if(*p==']'&& *(p+1)=='\0')									
		{		
			WriteBaudRateToEEPROM(baudrate);	
			return 1;		
		}
		UART_SendStr("<NA>\n");return 1;	
	}
	else return 0;	
}
//
uint8_t  CommmandLine_rom(char *Commands)
{
	if( strncmp(Commands,"rom[",4)==0  ) 
	{	
		char *p = NULL;
		char *s = &Commands[4];	
		u8 i;
		Data_4Bytes DataSet;		
		u16 DataNum=0;
		DataNum=strtol(s, &p, 10);	
		//UART_Printf("%c;%c",*p,*(p+1));	OUTPUT1=1;
		if(*p==']'&& *(p+1)=='[' && ( DataNum>0 && DataNum<101) )
		{
			s=NULL;
			DataSet.f32=strtod(p+2, &s);			
			if(*(s)==']'&& *(s+1)=='\0' )			
			{																	
				for(i=0;i<4;i++)
				{
					AT24CXX_WriteOneByte(DataNum*4+100+i,DataSet.Byte[i]);
				}
				//AT24CXX_WriteLenByte(DataNum*4+100,(u32)DataSet.f32,4);
				UART_Printf("ROM[%d][%.3f]<OK>\n",(u16)DataNum,DataSet.f32); 
				return 1;
			}
		}	
	}
	if( strncmp(Commands,"rom[",4)==0  ) 
	{	
		char *p = NULL;
		char *s = &Commands[4];	
		u8 i;
		u16 DataNum=0;	
		Data_4Bytes	DataReads;	
		DataNum=strtol(s, &p, 10);	
		if(*p==']'&& *(p+1)=='\0'	&& (DataNum>0 && DataNum<101))
		{	
			for(i=0;i<4;i++)
			{
				DataReads.Byte[i]=AT24CXX_ReadOneByte(DataNum*4+100+i);
			}
			UART_Printf("ROM[%d]<%.3f>\n",(u16)DataNum,DataReads.f32);
			return 1;
		}
		UART_SendStr("<NA>\n");return 1;
	}		
	else return 0;
	
}
//
uint8_t  CommmandLine_RomPage(char *Commands)
{
	if( strncmp(Commands,"save info ",10)==0  ) 
	{	
		char *p = NULL;
		char *s = &Commands[10];	
		u32 PageNum=0;	
		PageNum=strtol(s, &p, 10);	

		if(	*p==' ' && PageNum>0 && PageNum <(256-10)	)			//前10页用于FSN+ROM 
		{
			IIC_DeviceWriteBytes(0xA0,64*(10+PageNum),(u8 *)p+1,64,2);
			UART_Printf("save info %d OK\n",PageNum); 
			return 1;
		}	
	  UART_SendStr("<NA>\n");return 1;
	}
	if( strncmp(Commands,"read info ",10)==0  ) 
	{	
		char *p = NULL;
		char *s = &Commands[10];	
		u32 PageNum=0;	
		u8 DataRead[64];
		PageNum=strtol(s, &p, 10);	
		
		if(	*p=='\0' && PageNum>0 && PageNum <(256-10)	)			//前10页用于FSN+ROM 
		{
			IIC_DeviceReadBytes(0xA0,64*(10+PageNum),	DataRead,64,2);
			UART_Printf("read info %d %.64s\n",PageNum,DataRead);
			return 1;
		}	
	  UART_SendStr("<NA>\n");return 1;
	}
	else return 0;
}
//
