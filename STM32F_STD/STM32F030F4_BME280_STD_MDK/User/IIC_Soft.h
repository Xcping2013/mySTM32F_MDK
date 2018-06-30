#ifndef __IIC_SOFT_H
#define __IIC_SOFT_H

#include "delay.h"
//IIC

//IIC所有操作函数
void IIC_Init(void);                		//初始化IIC的IO口				 
void IIC_Start(void);										//发送IIC开始信号
void IIC_Stop(void);	  								//发送IIC停止信号
void IIC_Send_Byte(u8 txd);							//IIC发送一个字节
u8 	 IIC_Read_Byte(unsigned char ack);	//IIC读取一个字节
u8 	 IIC_Wait_Ack(void); 								//IIC等待ACK信号
void IIC_Ack(void);											//IIC发送ACK信号
void IIC_NAck(void);										//IIC不发送ACK信号

void IIC_DeviceReadBytes(uint8_t DeviceAddr, u16 ReadAddr, uint8_t *RegData, uint8_t len, uint8_t DeviceType );
void IIC_DeviceWriteBytes(uint8_t DeviceAddr, uint16_t RegAddr, uint8_t *RegData , uint8_t len, uint8_t DeviceType);

#endif
















