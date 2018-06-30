/**
  \file 	 Eeprom.h
  \author  xcping
  \version 2016/10/29 Ver1.00
  \brief 	 AT24C128 access functions
  This file contains the EEPROM access function defintions.
*/
#ifndef 	__EEPROM_H
	#define __EEPROM_H
	
#include "stm32f1xx_hal.h"

#define IIC_SDA_IN()  {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=0X00000008;}
#define IIC_SDA_OUT() {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=0X00000003;}

#define IIC_SCL    			PBout(9) //SCL
#define IIC_SDA    			PBout(8) //SDA	 
#define IIC_READ_SDA   	PBin(8)  //输入SDA 

#define EEPROM_ADDR			0xA0


//IIC所有操作函数	 
void WriteEepromByte(UINT Address, UCHAR Value);
void WriteEepromBlock(UINT Address, UCHAR *Block, UINT Size);
UCHAR ReadEepromByte(UINT Address);
void ReadEepromBlock(UINT Address, UCHAR *Block, UINT Size);
void EEPROM_Init(void);


#endif
