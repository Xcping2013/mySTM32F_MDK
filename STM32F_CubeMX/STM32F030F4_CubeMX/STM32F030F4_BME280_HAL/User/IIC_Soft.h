#ifndef __IIC_SOFT_H
#define __IIC_SOFT_H
#include "main.h"
#include "delay.h"
//IIC

//IIC���в�������
void IIC_Init(void);                		//��ʼ��IIC��IO��				 
void IIC_Start(void);										//����IIC��ʼ�ź�
void IIC_Stop(void);	  								//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);							//IIC����һ���ֽ�
u8 	 IIC_Read_Byte(unsigned char ack);	//IIC��ȡһ���ֽ�
u8 	 IIC_Wait_Ack(void); 								//IIC�ȴ�ACK�ź�
void IIC_Ack(void);											//IIC����ACK�ź�
void IIC_NAck(void);										//IIC������ACK�ź�

void IIC_DeviceReadBytes(uint8_t DeviceAddr, u16 ReadAddr, uint8_t *RegData, uint8_t len, uint8_t DeviceType );
void IIC_DeviceWriteBytes(uint8_t DeviceAddr, uint16_t RegAddr, uint8_t *RegData , uint8_t len, uint8_t DeviceType);

#endif
















