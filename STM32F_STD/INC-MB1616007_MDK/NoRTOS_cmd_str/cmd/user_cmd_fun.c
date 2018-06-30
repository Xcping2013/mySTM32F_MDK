#include "string.h"
#include "usart.h"
#include "at24cxx.h" 

#include "user_cmd_fun.h"
#include "delay.h" 

/******************--->	INPUT & OUTPUT	**************************
 */
uint8_t  CMD_int(char *Commands)
{
	if( strcmp(Commands,"inputs")==0  ) 
	{
		Int.readAll();
		printf("<%.16s>",Int.data_read);
		return 1;
	}
	if( strncmp(Commands,"int[",4)==0  ) 
	{	
		char *p = NULL;
		char *s = &Commands[4];			
		u8 channel=strtol(s, &p, 10);	
		if(*p==']'&& *(p+1)=='\0'	&& channel>0 && channel<17)
		{	
			printf("<%d>",Int.read(channel));	
			return 1;
		}
	}
  if((strncmp("input",Commands,5)==0) && (Commands[5]==' ') )
  {
		 u8 channel = ((Commands[6]-0x30)*10)+((Commands[7])-0x30);	
		 if(0<channel && channel<17)
		 {
			 if(Int.read(channel)==1)	  printf("\r\nsenser%d turn on", channel);
			 else												printf("\r\nsenser%d turn off",channel);
			return 1;				 
			}
	}
  else return 0;	
}
//
uint8_t  CMD_out(char *Commands)
{
	if( strncmp(Commands,"outputs[",8)==0  ) 
	{		
		if(strlen(Commands)==25 && Commands[24]	==']' && Commands[25]=='\0')
		{
			Out.writeAll(&Commands[8]);
			printf("<OK>");
			return 1;
		}
	}	
	if( strncmp(Commands,"out[",4)==0  ) 
	{	
		char *p = NULL;
		char *s = &Commands[4];		
		u8 setval;		
		u8 channel=strtol(s, &p, 10);	
		if(*p==']'&& *(p+1)=='[' && channel>0 && channel<=16)	
		{
			s=NULL;
			setval=strtol(p+2, &s, 10);	
			if(*(s)==']'&& *(s+1)=='\0' )
			{
				Out.write(channel, setval);
				printf("<OK>");
				return 1;
			}			
		}	
	}	
  if((strncmp("output",Commands,6)==0) && (Commands[6]==' ') )
  {
		 u8 channel = ((Commands[7]-0x30)*10)+((Commands[8])-0x30);
     u8 status	= Commands[10]-0x30;	
		if(0<channel && channel<17)
		{
			Out.write(channel, status); 
			printf(" OK");
			return 1;
		}
  }
  else return 0;	 
}
/******************			INPUT & OUTPUT<---**************************
*/

//
/******************--->	AT24CXXX_APP	 	**************************
 */
at24cxx_t at24c256=
{
	{0},
	{&PA0,&PC3},
	0xA2,
};
void eeprom_init(void)
{
	at24c256.eeprom=C24LC256;//C24LC04;
	at24cxx.init(at24c256);
}
//FSN---page0
uint8_t  CMD_fsn(char *Commands)
{
	if(strcmp("fsn",		Commands)==0)			
	{		
		uint8_t	i;
		uint8_t fsn[64];
		at24cxx.read(at24c256 , 0, fsn, 64);
		//at24lc04_reads(0);
		for(i=0;i<64;i++)	
		{
			if(fsn[i]>127) { fsn[i]='\0';break;}
		}
		printf("<%.64s>",fsn);
		return 1;	
	}
	if(	(Commands[0]=='s')	&&	(Commands[1]=='n')	&&	(Commands[2]==':')	)
	{
		at24cxx.write(at24c256, 0, &Commands[3], strlen(&Commands[3])+1);
		//at24lc04_writes(0,&Commands[3]);
		printf("<OK>");
		return 1;
	}

	else if(strncmp("fsn[",	Commands,4)==0)	
	{	
		uint8_t i;
		for(i=0;i<64;i++)
		{
			if(Commands[i+4]==']') {Commands[i+4]='\0';	break ;}
		}
		if(	i>0	&& i<64	&& Commands[i+4]=='\0'&& Commands[i+5]=='\0')			
		{		
			at24cxx.write(at24c256, 0, &Commands[4], strlen(&Commands[4])+1);
			//at24lc04_writes(0,&Commands[3]);
			printf("<OK>");
			return 1;		
		}
	}	
	else return 0;
}
//
uint8_t  CMD_rom(char *Commands)
{
	if( strncmp(Commands,"rom[",4)==0  ) 
	{	
		char *p = NULL;
		char *s = &Commands[4];	
		u8 DataSet;		
		u8 DataNum=0;
		DataNum=strtol(s, &p, 10);	
		if(*p==']'&& *(p+1)=='[' && ( DataNum>0 && DataNum<101 ))
		{
			s=NULL;
			DataSet=strtod(p+2, &s);			
			if(*(s)==']'&& *(s+1)=='\0' )			
			{																	
				*(s)='\0';
				at24cxx.write(at24c256,DataNum*64,p+2,strlen(p+2)+1);
				//at24lc04_writes(DataNum+16,p+2);
				printf("<OK>");
				return 1;
			}
		}	
	}
	if( strncmp(Commands,"rom[",4)==0  ) 
	{	
		char *p = NULL;
		char *s = &Commands[4];	
		u8 i;
		u8 rom[64];
		u16 DataNum=0;	
		DataNum=strtol(s, &p, 10);	
		if(*p==']'&& *(p+1)=='\0'	&& (DataNum>0 && DataNum<101))
		{	
			at24cxx.read(at24c256 , DataNum*64, rom, 64);
			for(i=0;i<64;i++)	
			{
				if(rom[i]>127) { rom[i]='\0';break;}
			}
			printf("<%.64s>",rom);
			//at24lc04_reads(DataNum+16);
			return 1;
		}
	}		
	else return 0;
	
}
//
uint8_t  CMD_RomPage(char *Commands)
{
	if( strncmp(Commands,"save info ",10)==0  ) 
	{	
		char *p = NULL;
		char *s = &Commands[10];	
		u32 PageNum=0;	
		PageNum=strtol(s, &p, 10);	

		if(	*p==' ' && PageNum>0 && PageNum <100	)			
		{
			at24cxx.write(at24c256,(PageNum+100)*64,p+1,strlen(p+1)+1);
			printf(" OK");
			//at24lc04_writes(PageNum,p+1);
			//Flash_write(PageNum,p+1);
			return 1;
		}	
	}
	if( strncmp(Commands,"read info ",10)==0  ) 
	{	
		char *p = NULL;
		char *s = &Commands[10];	
		u8 i;
		u32 PageNum=0;	
		u8 read_info[64];
		PageNum=strtol(s, &p, 10);	
		
		if(	*p=='\0' && PageNum>0 && PageNum <400	)			//Ç°100Ò³ÓÃÓÚFSN+ROM 
		{
			at24cxx.read(at24c256 , (PageNum+100)*64, read_info, 64);
			for(i=0;i<64;i++)	
			{
				if(read_info[i]>127) { read_info[i]='\0';break;}
			}
			printf(" %.64s",read_info);
			//at24lc04_reads(PageNum);
			//Flash_read(PageNum);
			return 1;
		}	
	}
	else return 0;
}
/******************			AT24CXXX_APP<---**************************
*/

