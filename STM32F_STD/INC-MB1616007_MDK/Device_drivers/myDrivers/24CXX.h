#ifndef __24CXX_H
#define __24CXX_H

#include "IIC_SOFT.h"

//	     		Kbit		Byte				pageSize					MaxFreq	 		Write Cycle   data 	word 			address 
#define AT24C01			127					//16*8		2**7			400K	
#define AT24C02			255					//32*8		2**8   		1MHZ			8-bit 
#define AT24C04			511					//32*16							1MHZ		        											1010A2A1P0W/R
#define AT24C08			1023				//64*16																										1010A2P1P0W/R
#define AT24C16			2047				//128*16																									1010P2P1P0W/R
#define AT24C32			4095				//128*32																									1010A2A1A0W/R
#define AT24C64	    8191
#define AT24C128		16383				//256*64Byte				12MHZ			5ms Max/100W 		14-bit
#define AT24C256		32767  

#define EE_TYPE 		AT24C128
					  
u8 AT24CXX_ReadOneByte(u16 ReadAddr);												//指定地址读取一个字节
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//指定地址开始写入指定长度的数据
u32  AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);									//指定地址开始读取指定长度数据
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据

u8 AT24CXX_Check(void);  //检查器件


uint8_t	MyCLI_rom(char *str);

#endif
















