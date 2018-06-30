#ifndef __IIC_SOFT_H
#define __IIC_SOFT_H

#include "stm32f1xx_hal.h"
#include "defines.h"
//IIC
#define IIC_SDA_IN()  {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=0X00000008;}
#define IIC_SDA_OUT() {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=0X00000003;}

#define IIC_SCL    			PBout(9) //SCL
#define IIC_SDA    			PBout(8) //SDA	 
#define IIC_READ_SDA   	PBin(8)  //输入SDA 

//IIC所有操作函数
//void IIC_Init(void);                		//初始化IIC的IO口				 
void IIC_Start(void);										//发送IIC开始信号
void IIC_Stop(void);	  								//发送IIC停止信号
void IIC_Send_Byte(u8 txd);							//IIC发送一个字节
u8 	 IIC_Read_Byte(unsigned char ack);	//IIC读取一个字节
u8 	 IIC_Wait_Ack(void); 								//IIC等待ACK信号
void IIC_Ack(void);											//IIC发送ACK信号
void IIC_NAck(void);										//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 	 IIC_Read_One_Byte(u8 daddr,u8 addr);	  

void IIC_ReadByte(uint8_t 	DeviceAddr, uint8_t Addr_Data );
void IIC_ReadBytes(uint8_t 	DeviceAddr, char * 	Addr_Data, 	uint8_t len );
void IIC_WriteByte(uint8_t 	DeviceAddr, uint8_t RegAddr, 		uint8_t RegData );
void IIC_WriteBytes(uint8_t DeviceAddr, uint8_t RegAddr, 		char *RegData , uint8_t len);


void IIC_DeviceReadBytes(uint8_t DeviceAddr, u16 ReadAddr, uint8_t *RegData, uint8_t len, uint8_t DeviceType );
void IIC_DeviceWriteBytes(uint8_t DeviceAddr, uint16_t RegAddr, uint8_t *RegData , uint8_t len, uint8_t DeviceType);

#endif
















