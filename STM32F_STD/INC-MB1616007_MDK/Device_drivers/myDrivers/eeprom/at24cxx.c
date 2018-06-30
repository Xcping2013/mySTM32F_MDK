#include "at24cxx.h" 
#include "delay.h"

#define min(X,Y)  ( (X) > (Y) ? 	(Y) : (X) ) 

const device_t C24LC01   = {AT24C01, 0x007F, 0x08, 0x01};
const device_t C24LC02   = {AT24C02, 0x00FF, 0x08, 0x01};
const device_t C24LC04   = {AT24C04, 0x01FF, 0x10, 0x02};
const device_t C24LC08   = {AT24C08, 0x03FF, 0x10, 0x04};
const device_t C24LC16   = {AT24C16, 0x07FF, 0x10, 0x08};
const device_t C24LC32   = {AT24C32, 0x0FFF, 0x20, 0x01};
const device_t C24LC64   = {AT24C64, 0x1FFF, 0x20, 0x01};
const device_t C24LC128  = {AT24C128,0x3FFF, 0x40, 0x01};
const device_t C24LC256  = {AT24C256,0x7FFF, 0x40, 0x01};
const device_t C24LC512  = {AT24C512,0xFFFF, 0x40, 0x01};

//初始化IIC接口
void AT24CXX_Init(at24cxx_t dev)
{
	SoftI2c.pin_init(dev.pins);
}
void AT24CXX_Reads(at24cxx_t dev, u16 address,u8 *data,	u16 length)
{
	if(address+length < (dev.eeprom.maxAddress + 2)	)
	{
		if (dev.eeprom.type < AT24C04)
		{
			SoftI2c.reads(dev.pins, 1, dev.devAddress, address ,data,length);
		}
		else if (dev.eeprom.type > AT24C02 && dev.eeprom.type < AT24C32)
		{
			SoftI2c.reads(dev.pins, 3, dev.devAddress, address ,data,length);
		}
		else 
		{
			SoftI2c.reads(dev.pins, 2, dev.devAddress, address ,data,length);
		}	
	}
}
/**
	ex:AT24C256_Writes(10,data,100):
	---1.	SizeLeft=100;				OffsetPage=10%64=10;	SizeNext=54 			write(10,&data[0],54);
	---2.	SizeLeft=100-54=46	OffsetPage=64%64=0;	 	SizeNext=46 			write(64,&data[54],46);
  ---3. SizeLeft==46-46=0		

 */
void AT24CXX_Writes(at24cxx_t dev, u16 address,u8 *data,	u16 length)
{	
	if(address+length < ( dev.eeprom.maxAddress + 2) )
	{
		s16 SizeLeft	 = length;						// bytes left to write
		s16 OffsetData = 0;									// current offset in data pointer
		s16 OffsetPage = 0;									// current offset in page
		s16 SizeNext 	 = 0;									// next n bytes to write	
		
		while(SizeLeft>0)									  //还有数据要写
		{	
			//页偏移地址 	
			OffsetPage	 = address % dev.eeprom.pageSize;		
			SizeNext 		 = min(min(SizeLeft, dev.eeprom.pageSize), dev.eeprom.pageSize - OffsetPage);		
			//从页偏移地址开始写入下次可写的长度
			if (dev.eeprom.type < AT24C04)
			{
				SoftI2c.writes(dev.pins, 1, dev.devAddress, address,(data+OffsetData),SizeNext);
			}
			else if (dev.eeprom.type > AT24C02 && dev.eeprom.type < AT24C32)
			{
				SoftI2c.writes(dev.pins, 3, dev.devAddress, address,(data+OffsetData),SizeNext);
			}
			else 
			{
				SoftI2c.writes(dev.pins, 2, dev.devAddress, address,(data+OffsetData),SizeNext);
			}
			delay_ms(5);
			SizeLeft-=SizeNext;							//剩下的字节数
			OffsetData+=SizeNext;						//下次需要写入的DATA位置
			address+=SizeNext;							//下次需要写入的ADDR位置	
		}
	}
}
//在AT24CXX指定地址读出一个数据
u8 AT24CXX_ReadU8(at24cxx_t dev, u16 address)
{				  
	AT24CXX_Reads(dev,address,dev.data,1);
	return dev.data[0];
}
/**
 * Write byte
 */
void AT24CXX_WriteU8(at24cxx_t dev, u16 address,u8 data)
{			
	AT24CXX_Writes(dev, address, &data,1);
	delay_ms(5);
}
//
u16 AT24CXX_ReadU16(at24cxx_t dev, u16 address)
{			
	AT24CXX_Reads(dev, address, dev.data, 2);
	return *(u16*)&dev.data[0];
}
//
void AT24CXX_WriteU16(at24cxx_t dev, u16 address,u16 data)
{			
	AT24CXX_Writes(dev, address, (u8*)&data,2);
	delay_ms(5);
}
/**
 * Read long
 */
u32 AT24CXX_ReadU32(at24cxx_t dev, u16 address)
{			
	AT24CXX_Reads(dev, address, dev.data, 4);
	return *(u32*)&dev.data[0];
}
/**
 * Write long
 */
void AT24CXX_WriteU32(at24cxx_t dev, u16 address,u32 data)
{		
	AT24CXX_Writes(dev, address, (u8*)&data,4);	
	delay_ms(5);
}

/**
 * Read float
 */
f32 AT24CXX_ReadF32(at24cxx_t dev, u16 address)
{		
	AT24CXX_Reads(dev, address, dev.data, 4);
	return *(float*)&dev.data[0];
}
/**
 * Write float
 */
void AT24CXX_WriteF32(at24cxx_t dev, u16 address,f32 data)
{			
	AT24CXX_Writes(dev, address, (u8*)&data,4);	
	delay_ms(5);
}

/**
 * Read chars
 */

/**********************************AT24CXX_FUN_APP**********************************
*/
AT24CXX_DrvTypeDef at24cxx=
{
	AT24CXX_Init,
	AT24CXX_Reads,
	AT24CXX_Writes,
	
	AT24CXX_ReadU8,
	AT24CXX_WriteU8,
	
	AT24CXX_ReadU16,
	AT24CXX_WriteU16,
	
	AT24CXX_ReadU32,
	AT24CXX_WriteU32,
	
	AT24CXX_ReadF32,
	AT24CXX_WriteF32,
};
/*********************************   END  *********************************
*/


