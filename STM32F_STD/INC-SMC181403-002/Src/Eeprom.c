/**
  \file 		Eeprom.c
  \author 	Xcping2016
  \version 	1.00
  \brief 
*/
#include "UART.h"
#include "Eeprom.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define DATAS_LEN		4

//参数成员
typedef struct
{
	unsigned char name[4];      //显示参数
	unsigned int  address;      //数据地址
	unsigned char leng;         //数据长度
	int           upper;        //数据下限
	int           lower;        //数据上限  
	int           value;        //设定值
	//unsigned char sign;         //正负
}DataItem;
//
const DataItem Datas[DATAS_LEN]=
{
					//Name				EEPROM_ADDR     Bytes      Upper     Lower     SetValue		sign	
/* 0 */		"HW"	,				ADDR_HW,				1,				 255,				0,				100,				//0,
/* 1 */		"SW"	,				ADDR_SW,				1,				 255,				0,				200,				//0,	
/* 2 */		"BT"	,				ADDR_BT,        4,      460800,    4800,   	 115200,		   	//0,        
/* 3 */		"DE"	,				ADDR_DE,        1,         255,       0,          1,     	  //0,	
};
///* 2 */		"FSN "	,		 ADDR_FSN,			 10,				 255,				0,					0,			//0,

/**	 Write Data to EEPROM Once
**
**/
uint8_t DefaultSettingsToEEPROM(void)
{
	if(AT24CXX_ReadOneByte(ADDR_Update)!=EEPROM_SET)
	{
		u8 i;	u8 DataNum;	
		Data_4Bytes DataSet;
		//UART_Printf("First time to write EEPROM\n");	
		for(i=0;i<DATAS_LEN;i++)	
		{
			AT24CXX_WriteLenByte(Datas[i].address, Datas[i].value,Datas[i].leng);
			AT24CXX_Write(ADDR_FSN, (u8*)"IN3-SMC-01",10);
			AT24CXX_WriteOneByte(ADDR_Update,EEPROM_SET);
		}
		DataSet.f32=200.00;
		for(DataNum=1;DataNum<4;DataNum++)
		{
			for(i=0;i<4;i++)
			{
				AT24CXX_WriteOneByte(DataNum*4+100+i,DataSet.Byte[i]);
			}
		}
		if(AT24CXX_ReadOneByte(ADDR_Update)==EEPROM_SET) 
			return 1;
		else return 0;
	}
	else return 1;
}
/**	 Get the SoftWare version number
**				110-1.10
**/
void PrintSoftVersionFromEEPROM(void)
{
	u8 SoftWareNum	=	AT24CXX_ReadOneByte(ADDR_SW);
	UART_Printf("SV<%01d.%01d%01d>\n",SoftWareNum/100,SoftWareNum%100/10,SoftWareNum%100%10);
}
/**	 Save the Hardware version number
**				110-1.10
**/
void WriteSWToEEPROM(uint8_t SoftWareNum)
{
	AT24CXX_WriteOneByte(ADDR_SW,SoftWareNum);
	UART_Printf("SV[%01d.%01d%01d]<OK>\n",SoftWareNum/100,SoftWareNum%100/10,SoftWareNum%100%10);
}
/**	 Get the SoftWare version number
**	 110-1.10
**/
void PrintHardVersionFromEEPROM(void)
{
	u8 HardWareNum	=	AT24CXX_ReadOneByte(ADDR_HW);
	UART_Printf("HV<%01d.%01d%01d>\n",HardWareNum/100,HardWareNum%100/10,HardWareNum%100%10);
}
/**	 Save the Hardware version number
**				110-1.10
**/
void WriteHWToEEPROM(uint8_t HardWareNum)
{
	AT24CXX_WriteOneByte(ADDR_HW,HardWareNum);
	UART_Printf("HV[%01d.%01d%01d]<OK>\n",HardWareNum/100,HardWareNum%100/10,HardWareNum%100%10);
}
/**	 Get FSN
**				
**/
void PrintFSNFromEEPROM(void)
{
	u8 ReadFsn[10];
	AT24CXX_Read(ADDR_FSN,ReadFsn,10);
	UART_Printf("FSN<%.10s>\n",ReadFsn);
}
/**	 Save FSN
**				
**/
void WriteFSNToEEPROM(uint8_t *Fsn)
{
	AT24CXX_Write(ADDR_FSN,Fsn,10);
	UART_Printf("FSN[%.10s]<OK>\n",Fsn);
}
/**	 Set BaudRate
**				
**/
void WriteBaudRateToEEPROM(uint32_t BaudRate)
{
	AT24CXX_WriteLenByte(ADDR_BT,BaudRate,4);
	UART_Printf("Baud[%d]<OK>\n",BaudRate);
}
//
//uint8_t ROM_WriteDatas(char *Commands)
//{
//	if( strncmp(Commands,"rom[",4)==0  ) 
//	{	
//		char *p = NULL;
//		char *s = &Commands[4];		
//		float DataSet;		
//		uint16_t DataNum=strtol(s, &p, 10);	
//		if(*p==']'&& *(p+1)=='[' &&  DataNum>0 && DataNum<=AT24C128)	
//		{
//			s=NULL;
//			DataSet=strtod(p+2, &s);	
//			if(*(s-1)==']'&& *(s)=='\0' )
//			{
//				AT24CXX_WriteLenByte(DataNum*4+100,DataSet,4);
//				UART_Printf("ROM[%d][%.4f]<OK>\n",(u16)DataNum,DataSet); 
//				return 1;
//			}
//		}	
//	}	
//	else return 0;
//}
////
//uint8_t ROM_ReadDatas(char *Commands)
//{
//	if( strncmp(Commands,"rom[",4)==0  ) 
//	{	
//		char *p = NULL;
//		char *s = &Commands[4];	
//		u16 DataNum=0;		
//		DataNum=strtol(s, &p, 10);	
//		if(*p==']'&& *(p+1)=='\0'	&& (DataNum>0 && DataNum<=AT24C128))
//		{	
//			
//			UART_Printf("ROM[%d]<%.4f>\n",(u16)DataNum,AT24CXX_ReadLenByte(DataNum*4+100,4));
//			return 1;
//		}
//		UART_SendStr("<NA>\n");return 1;
//	}	
//	else return 0;	
//}
//


